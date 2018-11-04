#include "stdafx.h"
#include "Player.h"
#include <strsafe.h>


HANDLE hProcess_;
DWORD* baseAddressPtr;
DWORD baseAddress;

boolean localPlayer = false;
float playerHeadPosition[3] = {};
float playerPosition[3] = {};
float cameraPosition[3] = {};
int health;
int armor;
int pistolAmmo1, pistolAmmo2;
int rifleAmmo1, rifleAmmo2;
int pistolCooldown;
int rifleCooldown;
int team = NULL;
char* name = nullptr;
//Weapon currentWeapon;

const int HEALTH_OFFSET = 0xF8;
const int TEAM_OFFSET = 0x32C;
const int POSITION_OFFSET = 0x34;
const int CAMERA_OFFSET = 0x40;



Player::Player(DWORD addressPtr, HANDLE _hProcess, boolean _localPlayer)
{
	hProcess_ = _hProcess;
	baseAddressPtr = (DWORD*)addressPtr;
	ReadProcessMemory(hProcess_, (BYTE*)baseAddressPtr, &baseAddress, sizeof(baseAddress), nullptr);
	localPlayer = _localPlayer;
	updateInfo();
}

Player::Player(DWORD address, HANDLE _hProcess)
{
	hProcess_ = _hProcess;
	baseAddress = address;
	updateInfo();
}


Player::~Player()
{
}

void Player::updateInfo()
{
	// Update Health
	ReadProcessMemory(hProcess_, (BYTE*)baseAddress + HEALTH_OFFSET, &health, sizeof(health), nullptr);

	// Update Team
	ReadProcessMemory(hProcess_, (BYTE*)baseAddress + TEAM_OFFSET, &team, sizeof(team), nullptr);

	// Update Position
	ReadProcessMemory(hProcess_, (BYTE*)baseAddress + POSITION_OFFSET, &playerPosition, sizeof(playerPosition), nullptr);
}

void Player::setCameraPosition(float x, float y, float z)
{
	cameraPosition[0] = x;
	cameraPosition[1] = y;
	cameraPosition[2] = z;
	WriteProcessMemory(hProcess_, (BYTE*)baseAddress + CAMERA_OFFSET, &cameraPosition, sizeof(cameraPosition), nullptr);
}

void Player::printInfo()
{
	std::cout << "PlayerBaseAddress: " << std::hex << this->baseAddress << std::dec << std::endl;
	std::cout << "Health: " << this->health << std::endl;
	std::cout << "Team: " << this->team << std::endl;
	std::cout << "PlayerPosition: x = " << this->playerPosition[0] << " y = " << this->playerPosition[1] << " z = " << this->playerPosition[2] << std::endl;
	std::cout << std::endl;
}

void Player::ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

