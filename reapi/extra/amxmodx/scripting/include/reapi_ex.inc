#if defined _reapi_ex_included
	#endinput
#endif

#define _reapi_ex_included

#if AMXX_VERSION_NUM >= 175
	#pragma reqlib reapi-ex
	#if !defined AMXMODX_NOAUTOLOAD
		#pragma loadlib reapi-ex
	#endif
#else
	#pragma library reapi-ex
#endif

enum hooks_tables_e_ex
{
	ht_engine_ex,
	ht_gamedll_ex,
	ht_player_ex,
};

#define BEGIN_FUNC_REGION_EX(%0)   (any:MAX_REGION_RANGE * hooks_tables_e_ex:ht_%0)
#define BEGIN_MEMBER_REGION_EX(%0) (any:MAX_REGION_RANGE * members_tables_e_ex:mt_%0)

#include <reapi>
#include <reapi_ex_version>
#include <reapi_ex_engine_const>
#include <reapi_ex_gamedll_const>

/*
* Hook API function that are available into enum.
* Look at the enums for parameter lists.
*
* @param function   The function to hook
* @param callback   The forward to call
* @param post       Whether or not to forward this in post
*
* @return           Returns a hook handle. Use EnableHookChain/DisableHookChain to toggle the forward on or off
*/
native HookChain:RegisterHookChainEx({EngineFuncEx, GamedllFuncEx, GamedllFunc_CBasePlayerEx}:function_id, const callback[], post = 0);

/*
* Stops a hook from triggering.
* Use the return value from RegisterHookChain as the parameter here!
*
* @param hook       The hook to stop
*
*/
native bool:DisableHookChainEx(HookChain:hook);

/*
* Starts a hook back up.
* Use the return value from RegisterHookChain as the parameter here!
*
* @param hook       The hook to re-enable
*
* @return           Returns true if the function is successfully executed, otherwise false
*/
native bool:EnableHookChainEx(HookChain:hook);

/*
* Sets the return value of a hookchain.
*
* @param type       To specify the ATYPE_* parameter, look at the enum AType
* @param value      The value to set the return to
*
*/
native SetHookChainReturnEx(AType:type, any:...);

/*
* Gets the return value of the current hookchain.
* This has no effect in pre hookchain.
*
* @param type       To specify the ATYPE_* parameter, look at the enum AType
* @param [maxlen]   Max length of string (optional)
*
* @return           If an integer or boolean or one byte or float, array or everything else is passed via 1st argument and more
*/
native any:GetHookChainReturnEx(AType:type, any:...);

/*
* Set hookchain argument.
* This has no effect in post hookchain.
*
* @param number     Number of argument
* @param value      New value
* @param [maxlen]   Max length of string (optional)
*
* @return           Returns true if the function is successfully executed, otherwise false
*/
native SetHookChainArgEx(number, AType:type, any:...);

/*
* Return call state of original API function (that are available into enum).
* Look at the enums for parameter lists.
*
* @param func       The function to get state
*
* @return           Returns true if the original function was called, otherwise false
*/
native bool:IsReapiHookOriginalWasCalledEx({EngineFuncEx, GamedllFuncEx, GamedllFunc_CBasePlayerEx}:function_id);

/*
* Returns the current hookchain handle.
*
* @return           Returns the hook handle
*/
native HookChain:GetCurrentHookChainHandleEx();

/*
* This is the callback from the module that gives major/minor versions for verifying compatibility for ReAPI versions.
* If an AMXX plugin gets a failure, then you do need to upgrade to the latest version of the ReAPI module or update the files included for AMXX plugins.
* Do not modify this!
*/
public __reapi_ex_version_check(const majorVersion, const minorVersion)
{
	if (majorVersion != REAPI_EX_VERSION_MAJOR)
	{
		new temp[512];
		formatex(temp, sizeof temp - 1, "[ReAPI-Ex]: Api major version mismatch; expected %d, real %d", REAPI_EX_VERSION_MAJOR, majorVersion);
		set_fail_state(temp);
		return;
	}

	if (minorVersion < REAPI_EX_VERSION_MINOR)
	{
		new temp[512];
		formatex(temp, sizeof temp - 1, "[ReAPI-Ex]: Api minor version mismatch; expected at least %d, real %d", REAPI_EX_VERSION_MINOR, minorVersion);
		set_fail_state(temp);
		return;
	}
}
/* AMXX-Studio Notes - DO NOT MODIFY BELOW HERE
*{\\ rtf1\\ ansi\\ deff0{\\ fonttbl{\\ f0\\ fnil Tahoma;}}\n\\ viewkind4\\ uc1\\ pard\\ lang3082\\ f0\\ fs16 \n\\ par }
*/
