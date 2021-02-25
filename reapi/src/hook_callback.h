#pragma once

#include <tuple>

// hookchain return type
enum HookChainState
{
	HC_CONTINUE = 0,	// plugin didn't take any action
	HC_SUPERCEDE,		// skip real function, use my return value
	HC_BREAK			// skip all forwards and real function, use my return value
						// @note Warning: Be very careful using this type of return will skip calls for all following AMXX the plugins.
};

// api types
enum AType : uint8
{
	ATYPE_INTEGER = 0,
	ATYPE_FLOAT,
	ATYPE_STRING,
	ATYPE_CLASSPTR,
	ATYPE_EDICT,
	ATYPE_EVARS,
	ATYPE_BOOL
};

struct retval_t
{
	bool set;
	AType type;

	union
	{
		char*			_string;
		float			_float;
		int				_integer;
		CBaseEntity*	_classptr;
		edict_t*		_edict;
		entvars_t*		_pev;
	};
};

inline AType getApiType(int)			{ return ATYPE_INTEGER; }
inline AType getApiType(unsigned)		{ return ATYPE_INTEGER; }
inline AType getApiType(float)			{ return ATYPE_FLOAT; }
inline AType getApiType(const char *)	{ return ATYPE_STRING; }
inline AType getApiType(char[])			{ return ATYPE_STRING; }
inline AType getApiType(CBaseEntity *)	{ return ATYPE_CLASSPTR; }
inline AType getApiType(edict_t *)		{ return ATYPE_EDICT; }
inline AType getApiType(entvars_t *)	{ return ATYPE_EVARS; }
inline AType getApiType(bool)			{ return ATYPE_BOOL; }

template<typename T>
inline AType getApiType(T *) { return ATYPE_INTEGER; }

inline bool hasStringArgs() { return false; }

template <typename T, typename ...f_args>
bool hasStringArgs(T, f_args... args)
{
	static_assert(sizeof(T) <= sizeof(int), "invalid hookchain argument size > sizeof(int)");

	if (getApiType(T()) == ATYPE_STRING)
		return true;

	return hasStringArgs(args...);
}

#define MAX_HOOKCHAIN_ARGS 12u

struct hookctx_t
{
	template<typename ...t_args>
	hookctx_t(size_t arg_count, t_args&&... args)
	{
		if (hasStringArgs(args...)) {
			tempstrings_used = 0;
		}

		args_count = min(arg_count, MAX_HOOKCHAIN_ARGS);
		setArgs(std::forward_as_tuple(args...));
	}

	template <size_t current = 0,
			typename tuple_t,
			size_t size = std::tuple_size<typename std::decay<tuple_t>::type>::value,
			std::enable_if_t<current >= size>* = nullptr> // if current >= size
	void setArgs(tuple_t &&t)
	{
	}

	template <size_t current = 0,
			typename tuple_t,
			size_t size = std::tuple_size<typename std::decay<tuple_t>::type>::value,
			std::enable_if_t<current < size>* = nullptr> // if current < size
	void setArgs(tuple_t &&t)
	{
		// current iteration
		if (current < MAX_HOOKCHAIN_ARGS)
		{
			auto &arg = std::get<current>(std::forward<tuple_t>(t));
			args[current].handle = (size_t)&arg;
			args[current].type   = getApiType(arg);
			setArgs<current + 1>(std::forward<tuple_t>(t)); // call next
		}
	}

	void reset(AType ret_type = ATYPE_INTEGER)
	{
		retVal.set = false;
		retVal.type = ret_type;
	}

	char* get_temp_string(AMX* amx)
	{
		auto ptr = s_temp_strings.push(amx);
		if (likely(ptr)) {
			tempstrings_used++;
			return ptr;
		}

		static char fatalErr[] = "<reapi error>";
		return fatalErr;
	}

	void SetId(int id) { index = id; }
	void ResetId()     { index = 0;  }

	void clear_temp_strings() const
	{
		s_temp_strings.pop(tempstrings_used);
	}

	int index                       = 0;
	retval_t retVal                 = {false,ATYPE_INTEGER};
	size_t tempstrings_used         = 0;

	struct args_t
	{
		size_t handle;
		AType  type;
	};

	size_t args_count               = 0;
	args_t args[MAX_HOOKCHAIN_ARGS] = {0u, ATYPE_INTEGER};
	static CTempStrings s_temp_strings;
};

extern hookctx_t* g_hookCtx;

template <typename original_t, typename ...f_args>
NOINLINE void DLLEXPORT _callVoidForward(hook_t* hook, original_t original, f_args&&... args)
{
	auto hookCtx = g_hookCtx;
	hookCtx->reset();
	int hc_state = HC_CONTINUE;

	hook->wasCalled = false;

	for (auto fwd : hook->pre)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret == HC_BREAK)) {
				return;
			}

			if (unlikely(ret > hc_state))
				hc_state = ret;
		}
	}

	if (hc_state != HC_SUPERCEDE) {
		g_hookCtx = nullptr;
		original(std::forward<f_args &&>(args)...);
		g_hookCtx = hookCtx;
		hook->wasCalled = true;
	}

	for (auto fwd : hook->post)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret == HC_BREAK))
				break;
		}
	}

	hook->wasCalled = false;
}

template <typename original_t, typename ...f_args>
void callVoidForward(size_t func, original_t original, f_args&&... args)
{
	hookctx_t hookCtx(sizeof...(args), args...);
	hookctx_t* save = g_hookCtx;

	g_hookCtx = &hookCtx;
	_callVoidForward(g_hookManager.getHookFast(func), original, args...);
	g_hookCtx = save;

	if (hasStringArgs(args...)) {
		hookCtx.clear_temp_strings();
	}
}

template <typename R, typename original_t, typename ...f_args>
NOINLINE R DLLEXPORT _callForward(hook_t* hook, original_t original, f_args&&... args)
{
	auto hookCtx = g_hookCtx;
	hookCtx->reset(getApiType(R()));

	int hc_state = HC_CONTINUE;

	hook->wasCalled = false;

	for (auto fwd : hook->pre)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret != HC_SUPERCEDE && ret != HC_BREAK)) {
				continue;
			}

			if (unlikely(!hookCtx->retVal.set)) {
				fwd->Error(AMX_ERR_ASSERT, "Can't suppress original function call without new return value set, so you must call SetHookChainReturn.");
				continue;
			}

			if (unlikely(ret == HC_BREAK)) {
				return *(R *)&hookCtx->retVal._integer;
			}

			if (unlikely(ret > hc_state))
				hc_state = ret;
		}
	}

	if (likely(hc_state != HC_SUPERCEDE))
	{
		g_hookCtx = nullptr;
		auto retVal = original(std::forward<f_args &&>(args)...);
		g_hookCtx = hookCtx;
		hook->wasCalled = true;

		if (unlikely(!hookCtx->retVal.set)) {
			switch (sizeof retVal) {
			case sizeof(int8):
				hookCtx->retVal._integer = *(int8 *)&retVal;
				break;
			case sizeof(int16):
				hookCtx->retVal._integer = *(int16 *)&retVal;
				break;
			case sizeof(int32):
				hookCtx->retVal._integer = *(int32 *)&retVal;
				break;
			}
			hookCtx->retVal.set = true;
		}
	}

	for (auto fwd : hook->post)
	{
		if (likely(fwd->GetState() == FSTATE_ENABLED))
		{
			hookCtx->SetId(fwd->GetIndex()); // set current handler hook
			auto ret = g_amxxapi.ExecuteForward(fwd->GetFwdIndex(), std::forward<f_args &&>(args)...);
			hookCtx->ResetId();

			if (unlikely(ret == HC_BREAK))
				break;
		}
	}
	
	hook->wasCalled = false;

	return *(R *)&hookCtx->retVal._integer;
}

template <typename R, typename original_t, typename ...f_args>
R callForward(size_t func, original_t original, f_args&&... args)
{
	static_assert(sizeof(R) <= sizeof(int), "invalid hookchain return type size > sizeof(int)");

	hookctx_t hookCtx(sizeof...(args), args...);
	hookctx_t* save = g_hookCtx;

	g_hookCtx = &hookCtx;
	auto ret = _callForward<R>(g_hookManager.getHookFast(func), original, args...);
	g_hookCtx = save;

	if (hasStringArgs(args...)) {
		hookCtx.clear_temp_strings();
	}

	return ret;
}

template<typename T, typename A>
struct hookdata_t
{
	hookdata_t(T chain, A args) : m_chain(chain), m_args(args) {}

	T m_chain;
	A m_args;
};

// rehlds functions
void ExecuteServerStringCmd(IRehldsHook_ExecuteServerStringCmd* chain, const char* text, cmd_source_t source, IGameClient* client);

struct SV_SendServerinfo_args_t
{
	SV_SendServerinfo_args_t(sizebuf_t* msg) : message(msg) {}

	sizebuf_t* message;
};

using SV_SendServerinfo_t = hookdata_t<IRehldsHook_SV_SendServerinfo*, SV_SendServerinfo_args_t&>;
void SV_SendServerinfo_AMXX(SV_SendServerinfo_t* data, IGameClient* cl);
void SV_SendServerinfo(IRehldsHook_SV_SendServerinfo* chain, sizebuf_t* msg, IGameClient* cl);
// regamedll functions
int GetForceCamera(IReGameHook_GetForceCamera *chain, CBasePlayer *pObserver);

// regamedll functions - player
void CBasePlayer_Spawn(IReGameHook_CBasePlayer_Spawn *chain, CBasePlayer *pthis);