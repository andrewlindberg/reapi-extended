#include "precompiled.h"

CTempStrings::CTempStrings()
{
	m_current = 0;
}


char* CTempStrings::push(AMX* amx)
{
	if (m_current == STRINGS_MAX) {
		AMXX_LogError(amx, AMX_ERR_NATIVE, "temp strings limit exceeded, contact reapi authors");
		return nullptr;
	}

	return m_strings[m_current++];
}

void CTempStrings::pop(size_t count)
{
	m_current -= count;
}

const char *getATypeStr(AType type)
{
	static constexpr const char *s_ATypes[]
	{
		"ATYPE_INTEGER",
		"ATYPE_FLOAT",
		"ATYPE_STRING",
		"ATYPE_CLASSPTR",
		"ATYPE_EDICT",
		"ATYPE_EVARS",
		"ATYPE_BOOL"
	};

	if (type >= arraysize(s_ATypes))
		return "";

	return s_ATypes[type];
}
