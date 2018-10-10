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


int main()
{
	// Get ProcID of target process
	DWORD processID = GetProcessID(L"ac_client.exe");

	// GetmoduleBaseAddress
	uintptr_t moduleBase = GetModuleBaseAddress(processID, L"ac_client.exe");

	// Get handle to to process
	HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, NULL, processID);

	World *world = new World(hProcess, moduleBase);
	Player *player = new Player(moduleBase + PLAYER_ENTITY_PTR_OFFSET, hProcess, true);
	
	std::vector<Player*> botList;
	for (int i = 0; i < world->playerArray.size(); i++)
	{
		Player *bot = new Player(world->playerArray[i], hProcess);
		botList.push_back(bot);
	}
	
	for (int i = 0; i < botList.size(); i++)
	{
		if (botList[i]->team == player->team)
		{
			botList.erase(botList.begin() + i);
		}
	}


	

	// Create dist func
	// Create enemy list
	// Loop through comparing team
	// sort array based on distance
	// If array.size > 0, write enemy position to player angle?





	CloseHandle(hProcess);
    return 0;
}

float calcDistance(Player player, Player enemy)
{
	return sqrt(pow(double(enemy.playerPosition[0] - player.playerPosition[0]), 2.0) +
				pow(double(enemy.playerPosition[1] - player.playerPosition[1]), 2.0) +
				pow(double(enemy.playerPosition[2] - player.playerPosition[2]), 2.0));
}

void disableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E4;

	WORD instruction = { 0x9090 };
	WriteProcessMemory(hProcess, (BYTE*)baseAddr, &instruction, sizeof(instruction), nullptr);
}

void enableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E4;

	WORD instruction = { 0x0A89 };
	WriteProcessMemory(hProcess, (BYTE*)baseAddr, &instruction, sizeof(instruction), nullptr);
}

void disableAmmoUse(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E9;

	WORD instruction = { 0x0000 };
	ReadProcessMemory(hProcess, (BYTE*)baseAddr, &instruction, sizeof(instruction), nullptr);
	std::cout << "instruction = " << std::hex << instruction << std::endl;

	WORD tmp = { 0x9090 };
	WriteProcessMemory(hProcess, (BYTE*)baseAddr, &tmp, sizeof(tmp), nullptr);
	
}

void allowAmmoUse(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E9;

	WORD instruction = { 0x0000 };
	ReadProcessMemory(hProcess, (BYTE*)baseAddr, &instruction, sizeof(instruction), nullptr);
	std::cout << "instruction = " << std::hex << instruction << std::endl;

	WORD tmp = { 0x0EFF };
	WriteProcessMemory(hProcess, (BYTE*)baseAddr, &tmp, sizeof(tmp), nullptr);

}

void writeAmmo(HANDLE hProcess, uintptr_t moduleBase)
{
	// Resolve Base Address of the pointer chain
	uintptr_t ammoPtrBaseAddr = moduleBase + 0x10f4f4;

	std::cout << "ammoPtrBaseAddr = " << "0x" << std::hex << ammoPtrBaseAddr << std::endl;

	// Resolve Ammo pointer chain
	std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
	uintptr_t ammoVA = FindDynamicMemAdress(hProcess, ammoPtrBaseAddr, ammoOffsets);

	std::cout << "ammoVA = " << "0x" << std::hex << ammoVA << std::endl;

	// Read ammo value
	int ammoValue = 0;

	ReadProcessMemory(hProcess, (BYTE*)ammoVA, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

	// Write to it
	int customAmmoValue = 1337;

	WriteProcessMemory(hProcess, (BYTE*)ammoVA, &customAmmoValue, sizeof(customAmmoValue), nullptr);

	// Read out again
	ReadProcessMemory(hProcess, (BYTE*)ammoVA, &ammoValue, sizeof(ammoValue), nullptr);
	std::cout << "New ammo = " << std::dec << ammoValue << std::endl;
}

void unlimitedHealth(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t healthPtrBaseAddr = moduleBase + 0x10f4f4;

	std::vector<unsigned int> healthOffsets = { 0xf8 };

	uintptr_t healthVA = FindDynamicMemAdress(hProcess, healthPtrBaseAddr, healthOffsets);

	int customHealthValue = 500;

	WriteProcessMemory(hProcess, (BYTE*)healthVA, &customHealthValue, sizeof(customHealthValue), nullptr);
}

