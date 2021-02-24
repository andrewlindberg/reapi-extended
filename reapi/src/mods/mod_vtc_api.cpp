#include "precompiled.h"

IVoiceTranscoderAPI *g_pVoiceTranscoderApi;

bool VTC_Api_Init()
{
	if (!g_RehldsApi)
		return false;

	g_pVoiceTranscoderApi = (IVoiceTranscoderAPI *)g_RehldsApi->GetFuncs()->GetPluginApi("VoiceTranscoder");

	if (!g_pVoiceTranscoderApi)
		return false;

	size_t majorVersion = g_pVoiceTranscoderApi->GetMajorVersion();
	size_t minorVersion = g_pVoiceTranscoderApi->GetMinorVersion();

	if (majorVersion != VOICETRANSCODER_API_VERSION_MAJOR)
	{
		UTIL_ServerPrint("[%s]: VTC API major version mismatch; expected %d, real %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MAJOR, majorVersion);

		// need to notify that it is necessary to update the VTC.
		if (majorVersion < VOICETRANSCODER_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the VTC up to a major version API >= %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MAJOR);
		}

		// need to notify that it is necessary to update the module.
		else if (majorVersion > VOICETRANSCODER_API_VERSION_MAJOR)
		{
			UTIL_ServerPrint("[%s]: Please update the %s up to a major version API >= %d\n", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < VOICETRANSCODER_API_VERSION_MINOR)
	{
		UTIL_ServerPrint("[%s]: VTC API minor version mismatch; expected at least %d, real %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MINOR, minorVersion);
		UTIL_ServerPrint("[%s]: Please update the VTC up to a minor version API >= %d\n", Plugin_info.logtag, VOICETRANSCODER_API_VERSION_MINOR);
		return false;
	}

	return true;
}
