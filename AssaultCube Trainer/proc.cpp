#include "stdafx.h"
#include "proc.h"
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD GetProcessID(const wchar_t* procName)
{
	HANDLE hProcessSnap;
	DWORD processID;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //0 indicates current process, but isn't used with _SNAPPROCESS
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		pe32.dwSize = sizeof(PROCESSENTRY32); //Size of structure MUST be definded before use

		if (Process32First(hProcessSnap, &pe32))
		{
			int tmp;
			do
			{
				tmp = _wcsicmp(pe32.szExeFile, procName);
				if(tmp == 0) //Equal
				{
					processID = pe32.th32ProcessID;
					break;
				}

			} while (Process32Next(hProcessSnap, &pe32));

		}
	}
	CloseHandle(hProcessSnap);
	return processID;
}

uintptr_t GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName)
{
	HANDLE hModuleSnap;
	uintptr_t baseAddress;
	MODULEENTRY32 me32;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);
	if (hModuleSnap != INVALID_HANDLE_VALUE)
	{
		me32.dwSize = sizeof(MODULEENTRY32); //Size of structure MUST be definded before use

		if (Module32First(hModuleSnap, &me32))
		{
			int tmp;
			do
			{
				tmp = _wcsicmp(me32.szModule, moduleName);
				if (tmp == 0) //Equal
				{
					baseAddress = (uintptr_t)me32.modBaseAddr;
					break;
				}

			} while (Module32Next(hModuleSnap, &me32));

		}
	}
	CloseHandle(hModuleSnap);
	return baseAddress;
}

uintptr_t FindDynamicMemAdress(HANDLE hProcess, uintptr_t baseAddrPtr, std::vector<unsigned int> offsets)
{
	uintptr_t address = baseAddrPtr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hProcess, (BYTE*)address, &address, sizeof(address), 0);
		address += offsets[i];
	}
	return address;
}
