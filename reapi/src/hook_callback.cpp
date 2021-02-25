#include "precompiled.h"

hookctx_t* g_hookCtx = nullptr;
CTempStrings hookctx_t::s_temp_strings;

/*
* ReHLDS functions
*/
void ExecuteServerStringCmd(IRehldsHook_ExecuteServerStringCmd* chain, const char* text, cmd_source_t source, IGameClient* client) {
	auto original = [chain](const char* _text, cmd_source_t _source, int _client)
	{
		chain->callNext(_text, _source, g_RehldsSvs->GetClient(_client));
	};

	callVoidForward(RH_ExecuteServerStringCmd, original, text, source, client ? client->GetId() + 1 : 0);
}

void SV_SendServerinfo_AMXX(SV_SendServerinfo_t* data, IGameClient* cl) {
	auto original = [data](int _cl)
	{
		data->m_chain->callNext(data->m_args.message, g_RehldsSvs->GetClient(_cl - 1));
	};

	callVoidForward(RH_SV_SendServerinfo, original, cl->GetId() + 1);
}

void SV_SendServerinfo(IRehldsHook_SV_SendServerinfo* chain, sizebuf_t* msg, IGameClient* cl) {
	SV_SendServerinfo_args_t args(msg);
	SV_SendServerinfo_t data(chain, args);
	SV_SendServerinfo_AMXX(&data, cl);
}

/*
* ReGameDLL functions
*/
int GetForceCamera(IReGameHook_GetForceCamera* chain, CBasePlayer* pObserver)
{
	auto original = [chain](int _pObserver)
	{
		return chain->callNext(getPrivate<CBasePlayer>(_pObserver));
	};

	return callForward<int>(RG_GetForceCamera, original, indexOfEdict(pObserver->pev));
}

void CBasePlayer_Spawn(IReGameHook_CBasePlayer_Spawn* chain, CBasePlayer* pthis)
{
	auto original = [chain](int _pthis)
	{
		chain->callNext(getPrivate<CBasePlayer>(_pthis));
	};

	callVoidForward(RG_CBasePlayer_Spawn, original, indexOfEdict(pthis->pev));
}
