#pragma once
#include <Windows.h>

class CSignature
{
public:
	DWORD dwFindPattern(DWORD dwAddress, DWORD dwLength, const char* szPattern);
	HMODULE GetModuleHandleSafe(const char* pszModuleName);
	DWORD GetClientSignature(const char* chPattern);
	DWORD GetEngineSignature(const char* chPattern);
	DWORD GetModuleSignature(const char* modulename, const char* chPattern);
};

extern CSignature gSignatures;
