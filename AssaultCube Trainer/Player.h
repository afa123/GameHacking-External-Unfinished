#include <Windows.h>
#include "World.h"
#include "proc.h"
#include <iostream>
#include <strsafe.h>
#pragma once
class Player
{
private:
public:
	DWORD baseAddress;

	float playerHeadPosition[3] = {};
	float playerPosition[3] = {};
	int health;
	int armor;
	int team = NULL;
	char* name = nullptr;


	Player(DWORD addressptr, HANDLE _hProcess, boolean _localPlayer);
	Player(DWORD address, HANDLE _hProcess);
	~Player();
	void updateInfo();
	void setCameraPosition(float x, float y, float z);
	void printInfo();
	void ErrorExit(LPTSTR lpszFunction);

};

