#pragma once

// reapi version
#include "reapi_version.inc"

class CAPI_Config {
public:
	CAPI_Config();
	void Init();
	void FailedReGameDllAPI();

	bool hasReHLDS() const { return m_api_rehlds; }
	bool hasReGameDLL() const { return m_api_regame; }
	
	void ServerDeactivate() const;

private:
	// to provide API functions
	bool m_api_rehlds;		// some useful functions
	bool m_api_regame;		// some useful functions #2
};

extern CAPI_Config api_cfg;
