// AssaultCube Trainer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <vector>
#include "proc.h"
#include <iostream>
#include "World.h"
#include "Player.h"
#include <cmath>

const int PLAYER_ENTITY_PTR_OFFSET = 0x10F4F4;

void disableRecoil(HANDLE hProcess, uintptr_t moduleBase);
void disableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase);
void writeToHealth(HANDLE hProcess, uintptr_t moduleBase);
void disableAmmoUse(HANDLE hProcess, uintptr_t moduleBase);

bool b_unlimitedAmmo = false;
bool b_writeToHealth = false;
bool b_noShotCooldown = false;
bool b_noRecoil = false;


int main()
{
	// Get ProcID of target process
	DWORD processID = GetProcessID(L"ac_client.exe");

	// GetmoduleBaseAddress
	uintptr_t moduleBase = GetModuleBaseAddress(processID, L"ac_client.exe");

	// Get handle to to process
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, NULL, processID);

	
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			b_unlimitedAmmo = !b_unlimitedAmmo;
			disableAmmoUse(hProcess, moduleBase);
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			b_noShotCooldown = !b_noShotCooldown;
			disableAssaultCooldown(hProcess, moduleBase);
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			b_noRecoil = !b_noRecoil;
			disableRecoil(hProcess, moduleBase);
		}
		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			b_writeToHealth = !b_writeToHealth;
		}
		if (b_writeToHealth)
		{
			writeToHealth(hProcess, moduleBase);
		}
		
		system("CLS");
		std::cout << "Unlimited Ammo: " << b_unlimitedAmmo << std::endl;
		std::cout << "No Shot Cooldown: " << b_noShotCooldown << std::endl;
		std::cout << "No Recoil: " << b_noRecoil << std::endl;
		std::cout << "WriteToHealth: " << b_writeToHealth << std::endl;
	}
	CloseHandle(hProcess);
    return 0;
}

void disableRecoil(HANDLE hProcess, uintptr_t moduleBase)
{
	
}

void disableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase)
{
	
}

void disableAmmoUse(HANDLE hProcess, uintptr_t moduleBase)
{
	
}

void writeToHealth(HANDLE hProcess, uintptr_t moduleBase)
{

}

