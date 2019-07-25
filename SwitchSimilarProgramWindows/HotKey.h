#pragma once
#include "framework.h"

#define HOTKEY_SWITCH_WINDOW	0x1
#define HOTKEY_OPEN_NEW_WINDOW	0x2

VOID SetHotKey(HWND hWnd);
VOID UnsetHotKey(HWND hWnd);