#include "HotKey.h"


VOID SetHotKey(HWND hWnd)
{
	if (RegisterHotKey(hWnd, 0x1, MOD_ALT, VK_OEM_3) == FALSE)
	{
		MessageBoxA(NULL, "×¢²á[Alt+`]ÈÈ¼üÊ§°Ü", "´íÎó", MB_OK | MB_ICONWARNING);
	}
	if (RegisterHotKey(hWnd, 0x2, MOD_CONTROL, 'N') == FALSE)
	{
		MessageBoxA(NULL, "×¢²á[Ctrl+N]ÈÈ¼üÊ§°Ü", "´íÎó", MB_OK | MB_ICONWARNING);
	}
}

VOID UnsetHotKey(HWND hWnd)
{
	UnregisterHotKey(hWnd, HOTKEY_SWITCH_WINDOW);
	UnregisterHotKey(hWnd, HOTKEY_OPEN_NEW_WINDOW);
}