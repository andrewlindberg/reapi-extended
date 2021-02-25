#if defined _reapi_ex_engine_const_included
	#endinput
#endif

#define _reapi_ex_engine_const_included

/**
* enum EngineFuncEx
*/
enum EngineFuncEx
{
	/*
	* Description: -
	*	cmd_source_t == 0 // came in over a net connection as a clc_stringcmd. host_client will be valid during this state. client == player index
	*	cmd_source_t == 1 // from the command buffer. client == 0
	* Params:       (const cmd[], const cmd_source_t, const client)
	*/
	RH_ExecuteServerStringCmd = BEGIN_FUNC_REGION_EX(engine),

	/*
	* Description: -
	* Params:       (const client)
	*/
	RH_SV_SendServerinfo,


};