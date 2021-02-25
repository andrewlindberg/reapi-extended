#if defined _reapi_ex_gamedll_const_included
	#endinput
#endif

#define _reapi_ex_gamedll_const_included

/**
* GamedllFuncEx
*/
enum GamedllFuncEx
{
	/*
	* Description:      -
	* Return type:      int
	* Params:           (const pObserver)
	*/
	RG_GetForceCamera = BEGIN_FUNC_REGION_EX(gamedll),
};

/**
* GamedllFunc CBasePlayer
*/
enum GamedllFunc_CBasePlayerEx
{
	/*
	* Description:      -
	* Params:           (const this)
	*/
	RG_CBasePlayer_Spawn = BEGIN_FUNC_REGION_EX(player),
};