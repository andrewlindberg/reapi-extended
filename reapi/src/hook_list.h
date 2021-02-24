#pragma once

#define MAX_HOOK_FORWARDS		1024
#define MAX_REGION_RANGE		1024

#define BEGIN_FUNC_REGION(x)		(MAX_REGION_RANGE * hooklist_t::hooks_tables_e::ht_##x)

typedef bool (*reqfunc_t) ();
typedef int  (*regfunc_t) (AMX *, const char *);
typedef void (*regchain_t)();

struct hook_t
{
	std::vector<class CAmxxHookBase *> pre;   // pre forwards
	std::vector<class CAmxxHookBase *> post;  // post forwards

	const char *func_name;                  // function name
	const char *depend_name;                // platform dependency

	reqfunc_t checkRequirements;
	regfunc_t registerForward;              // AMXX forward registration function
	regchain_t registerHookchain;           // register re* API hook
	regchain_t unregisterHookchain;         // unregister re* API hook

	void clear();

	bool wasCalled;
};

extern hook_t hooklist_engine[];
extern hook_t hooklist_gamedll[];
/*extern hook_t hooklist_animating[];
extern hook_t hooklist_player[];
extern hook_t hooklist_gamerules[];
extern hook_t hooklist_rechecker[];
extern hook_t hooklist_grenade[];
extern hook_t hooklist_weaponbox[];
extern hook_t hooklist_weapon[];
extern hook_t hooklist_gib[];
*/

enum
{
	INVALID_HOOKCHAIN = 0
};

struct hooklist_t
{
	hook_t *operator[](size_t hook) const
	{
		#define CASE(h)	case ht_##h: return &hooklist_##h[index];

		const auto table = hooks_tables_e(hook / MAX_REGION_RANGE);
		const auto index = hook & (MAX_REGION_RANGE - 1);

		switch (table) {
			CASE(engine)
			CASE(gamedll)
			/*CASE(animating)
			CASE(player)
			CASE(gamerules)
			CASE(rechecker)
			CASE(grenade)
			CASE(weaponbox)
			CASE(weapon)
			CASE(gib)*/
		}

		#undef CASE

		return nullptr;
	}

	static hook_t *getHookSafe(size_t hook);
	static void clear();

	enum hooks_tables_e
	{
		ht_engine,
		ht_gamedll,
		ht_animating,
		ht_player,
		ht_gamerules,
		ht_rechecker,
		ht_grenade,
		ht_weaponbox,
		ht_weapon,
		ht_gib,
	};
};

enum EngineFunc
{
	RH_SV_StartSound = BEGIN_FUNC_REGION(engine),
	RH_ExecuteServerStringCmd,
	// [...]
};

enum GamedllFunc
{
	RG_GetForceCamera = BEGIN_FUNC_REGION(gamedll),
	// [...]
};

enum GamedllFunc_CBasePlayer
{
	// CBasePlayer virtual
	RG_CBasePlayer_Spawn = BEGIN_FUNC_REGION(player),
	// [...]
};