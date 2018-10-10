#include "stdafx.h"
#include "World.h"

HANDLE hProcess = nullptr;
DWORD moduleBase = NULL;
int playerCount = NULL;
int oldPlayerCount = NULL;
std::vector<DWORD> playerArray;
DWORD playerArrayBaseAddr = NULL;

const int PLAYER_ARRAY_PTR_OFFSET = 0x10F4F8;
//std::vector<unsigned int> PLAYER_ARRAY_PTR_OFFSET = { 0x10F4F8, 0x0 };
const int PLAYER_COUNT_OFFSET = 0x10F500;

World::World(HANDLE _hProcess, DWORD _moduleBase)
{
	hProcess = _hProcess;
	moduleBase = _moduleBase;


	ReadProcessMemory(hProcess, (BYTE*)moduleBase + PLAYER_ARRAY_PTR_OFFSET, &playerArrayBaseAddr, sizeof(playerArrayBaseAddr), nullptr);
	playerArrayBaseAddr += 0x4;

	// Init playerCount and playerArray
	ReadProcessMemory(hProcess, (BYTE*)moduleBase + PLAYER_COUNT_OFFSET, &playerCount, sizeof(playerCount), nullptr);
	
	for (int i = 0; i < playerCount; i++)
	{
		DWORD EnemyEntityPtr;
		ReadProcessMemory(hProcess, (BYTE*)playerArrayBaseAddr + i*4, &EnemyEntityPtr, sizeof(EnemyEntityPtr), nullptr);
		playerArray.push_back(EnemyEntityPtr);
	}
}


World::~World()
{
}

void World::updateInfo()
{
	oldPlayerCount = playerCount;
	// Update playerCount
	ReadProcessMemory(hProcess, (BYTE*)moduleBase + PLAYER_COUNT_OFFSET, &playerCount, sizeof(playerCount), nullptr);

	if (oldPlayerCount != playerCount)
	{
		
		playerArray.clear();
		for (int i = 0; i < playerCount; i++)
		{
			DWORD tmpValue;
			ReadProcessMemory(hProcess, (BYTE*)playerArrayBaseAddr + (i * 4), &tmpValue, sizeof(tmpValue), nullptr);
			playerArray.push_back(tmpValue);
		}
	}

	// Update playerArray
	ReadProcessMemory(hProcess, (BYTE*)moduleBase + PLAYER_ARRAY_PTR_OFFSET, &playerArray, sizeof(playerArray[playerCount]), nullptr);
}

