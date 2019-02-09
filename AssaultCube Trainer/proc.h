#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcessID(const wchar_t* procName);
uintptr_t GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName);
uintptr_t resolvePointerChain(HANDLE hProcess, uintptr_t baseAddrPtr, std::vector<unsigned int> offsets);

