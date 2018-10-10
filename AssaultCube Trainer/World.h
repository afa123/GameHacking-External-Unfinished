#include <Windows.h>
#include <vector>
#include "proc.h"
#pragma once
class World
{
public:
	HANDLE hProcess = nullptr;
	DWORD moduleHandle = NULL;
	int playerCount = NULL;
	int oldPlayerCount = NULL;
	std::vector<DWORD> playerArray;

	const int PLAYER_ARRAY_PTR_OFFSET = 0x10F4F8;
	const int PLAYER_COUNT_OFFSET = 0x10F500;

	World(HANDLE hProcess, DWORD moduleBase);
	~World();
	void updateInfo();
};

