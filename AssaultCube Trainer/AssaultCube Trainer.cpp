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

void calcAngle(Player* player, Player* enemy);
void disableRecoil(HANDLE hProcess, uintptr_t moduleBase);
void disableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase);
void unlimitedHealth(HANDLE hProcess, uintptr_t moduleBase);
void disableAmmoUse(HANDLE hProcess, uintptr_t moduleBase);
float calcDistance(Player* player, Player* enemy);

bool b_unlimitedAmmo = false;
bool b_unlimitedHealth = false;
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
	
	
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			b_unlimitedAmmo = !b_unlimitedAmmo;
			//std::cout << "Unlimited Ammo: " << b_unlimitedAmmo << std::endl;
			disableAmmoUse(hProcess, moduleBase);
		}
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			b_noShotCooldown = !b_noShotCooldown;
			//std::cout << "No Shot Cooldown: " << b_noShotCooldown << std::endl;
			disableAssaultCooldown(hProcess, moduleBase);
		}
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			b_noRecoil = !b_noRecoil;
			//std::cout << "No Recoil: " << b_noRecoil << std::endl;
			disableRecoil(hProcess, moduleBase);
		}
		float lowest;
		int index;
		player->updateInfo();
		for (int i = 0; i < botList.size(); i++)
		{
			botList[i]->updateInfo();
			if (i == 0)
			{
				lowest = calcDistance(player, botList[0]);
				index = 0;
				continue;
			}
			float tmp = calcDistance(player, botList[i]);
			if (tmp < lowest)
			{
				lowest = tmp;
				index = i;
			}
		}
		calcAngle(player, botList[index]);
		
		system("CLS");
		std::cout << "Unlimited Ammo: " << b_unlimitedAmmo << std::endl;
		std::cout << "No Shot Cooldown: " << b_noShotCooldown << std::endl;
		std::cout << "No Recoil: " << b_noRecoil << std::endl;
		std::cout << "" << b_noRecoil << std::endl;
		player->printInfo();
		std::cout << "Distance: " << " " << lowest << std::endl;
		Sleep(5);
	}
	CloseHandle(hProcess);
    return 0;
}

void print()
{
	std::cout << "Unlimited Ammo: " << std::endl;
	std::cout << "No Shot Cooldown: " << std::endl;
	std::cout << "No Recoil: " << std::endl;
	std::cout << "" << std::endl;
	std::cout << "No Recoil: " << std::endl;

}

void calcAngle(Player* player, Player* enemy)
{
	float x, y, z;
	x = player->playerPosition[0] - enemy->playerPosition[0];
	y = player->playerPosition[1] - enemy->playerPosition[1];
	z = player->playerPosition[2] - enemy->playerPosition[2];

	player->setCameraPosition(x, y, z);



	// calculate distance between x, y and z
	// calculate hypotenuse
	// 6.42
}
float calcDistance(Player* player, Player* enemy)
{
	return sqrt(pow(double(enemy->playerPosition[0] - player->playerPosition[0]), 2.0) +
				pow(double(enemy->playerPosition[1] - player->playerPosition[1]), 2.0) +
				pow(double(enemy->playerPosition[2] - player->playerPosition[2]), 2.0));
}

void disableRecoil(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x63786;

	if (b_noRecoil)
	{
		BYTE disable[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &disable, sizeof(disable), nullptr);
	}
	else
	{
		BYTE enable[] = { 0x50, 0x8D, 0x4C, 0x24, 0x1C, 0x51, 0x8B, 0xCE, 0xFF, 0xD2 };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &enable, sizeof(enable), nullptr);
	}
}

void disableAssaultCooldown(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E4;
	if (b_noShotCooldown)
	{
		BYTE enable[] = { 0x90, 0x90 };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &enable, sizeof(enable), nullptr);
	}
	else
	{
		BYTE disable[] = { 0x89, 0x0A };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &disable, sizeof(disable), nullptr);
	}
}

void disableAmmoUse(HANDLE hProcess, uintptr_t moduleBase)
{
	uintptr_t baseAddr = moduleBase + 0x637E9;
	if (b_unlimitedAmmo)
	{
		BYTE enable[] = { 0x90, 0x90 };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &enable, sizeof(enable), nullptr);
	}
	else
	{
		BYTE disable[] = { 0xFF, 0x0E };
		WriteProcessMemory(hProcess, (BYTE*)baseAddr, &disable, sizeof(disable), nullptr);
	}
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

	DWORD customHealthValue = 0x1F4;
	std::cout << "test" << std::endl;
	WriteProcessMemory(hProcess, (BYTE*)healthVA, &customHealthValue, sizeof(customHealthValue), nullptr);
}

