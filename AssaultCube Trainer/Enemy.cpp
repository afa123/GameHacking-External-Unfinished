#include "stdafx.h"
#include "Enemy.h"

extern HANDLE _ProcessHandle;

DWORD* baseAddressPtr;
DWORD baseAddress;

Enemy::Enemy(DWORD address)
{
	baseAddressPtr = (DWORD*)address;
	ReadProcessMemory(_ProcessHandle, (BYTE*)baseAddressPtr, &baseAddress, sizeof(baseAddress), nullptr);
	updateInfo();
}


Enemy::~Enemy()
{
}

void Enemy::updateInfo()
{

}