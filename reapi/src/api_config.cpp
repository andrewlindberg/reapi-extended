#include "precompiled.h"

CAPI_Config api_cfg;

CAPI_Config::CAPI_Config() : m_api_rehlds(false), m_api_regame(false)
{

}

void CAPI_Config::FailedReGameDllAPI()
{
	m_api_regame = false;
}

void CAPI_Config::Init()
{
	m_api_rehlds    = RehldsApi_Init();
	m_api_regame    = RegamedllApi_Init();
	
	if (m_api_regame) {
		g_ReGameHookchains->InstallGameRules()->registerHook(&InstallGameRules);
	}
}

void CAPI_Config::ServerDeactivate() const
{
	if (m_api_regame) {
		g_pGameRules = nullptr;
	}
}
