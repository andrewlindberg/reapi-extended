#pragma once

template <typename T, size_t N>
char(&ArraySizeHelper(T(&array)[N]))[N];
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

extern enginefuncs_t* g_pengfuncsTable;
extern DLL_FUNCTIONS *g_pFunctionTable;

inline void EMESSAGE_BEGIN(int msg_dest, int msg_type, const float *pOrigin = nullptr, edict_t *ed = nullptr) { (*g_pengfuncsTable->pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed); }
inline void EMESSAGE_END() { (*g_pengfuncsTable->pfnMessageEnd)(); }
inline void EWRITE_BYTE(int iValue) { (*g_pengfuncsTable->pfnWriteByte)(iValue); }
inline void EWRITE_CHAR(int iValue) { (*g_pengfuncsTable->pfnWriteChar)(iValue); }
inline void EWRITE_SHORT(int iValue) { (*g_pengfuncsTable->pfnWriteShort)(iValue); }
inline void EWRITE_LONG(int iValue) { (*g_pengfuncsTable->pfnWriteLong)(iValue); }
inline void EWRITE_ANGLE(float flValue) { (*g_pengfuncsTable->pfnWriteAngle)(flValue); }
inline void EWRITE_COORD(float flValue) { (*g_pengfuncsTable->pfnWriteCoord)(flValue); }
inline void EWRITE_STRING(const char *sz) { (*g_pengfuncsTable->pfnWriteString)(sz); }
inline void EWRITE_ENTITY(int iValue) { (*g_pengfuncsTable->pfnWriteEntity)(iValue); }

void UTIL_ServerPrint(const char *fmt, ...);

const char *getATypeStr(AType type);

extern void NORETURN UTIL_SysError(const char *fmt, ...);
