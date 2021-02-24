#include "precompiled.h"

hookctx_t* g_hookCtx = nullptr;
CTempStrings hookctx_t::s_temp_strings;

/*
* ReHLDS functions
*/
void SV_StartSound(IRehldsHook_SV_StartSound *chain, int recipients, edict_t *entity, int channel, const char *sample, int volume, float attenuation, int fFlags, int pitch)
{
	auto original = [chain](int _recipients, int _entity, int _channel, const char *_sample, int _volume, float _attenuation, int _fFlags, int _pitch)
	{
		chain->callNext(_recipients, edictByIndexAmx(_entity), _channel, _sample, _volume, _attenuation, _fFlags, _pitch);
	};

	callVoidForward(RH_SV_StartSound, original, recipients, indexOfEdict(entity), channel, sample, volume, attenuation, fFlags, pitch);
}

void ExecuteServerStringCmd(IRehldsHook_ExecuteServerStringCmd* chain, const char* text, cmd_source_t source, IGameClient* client) {
	auto original = [chain](const char* _text, cmd_source_t _source, int _client)
	{
		chain->callNext(_text, _source, g_RehldsSvs->GetClient(_client));
	};

	callVoidForward(RH_ExecuteServerStringCmd, original, text, source, client ? client->GetId() + 1 : 0);
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
