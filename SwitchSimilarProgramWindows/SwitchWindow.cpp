#include "SwitchWindow.h"
#include <psapi.h>
//#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>

typedef struct _ENUM_WINDOW_PARAM
{
	TCHAR tszExeName[512];
	//TCHAR tszWindowName[512];
	TCHAR tszWindowClass[512];
	HWND hWnd;
	HWND hWnds[128];
	DWORD dwIndex = 0;
} PENUMWINDOWPARAM, * LPENUMWINDOWPARAM;

VOID GetExeName(HWND hWnd, LPTSTR dest, int size)
{
	DWORD dwProcessId = 0;

	DWORD dwRet = GetWindowThreadProcessId(hWnd, &dwProcessId);
	HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

	GetModuleFileNameEx(hProcess, NULL, dest, size);
	CloseHandle(hProcess);
}

/*
HWND _GetTopWindow(HWND hWNd)
{
	HWND hWndParent = hWNd;
	do
	{
		hWndParent = GetParent(hWndParent);
		if (NULL != hWndParent)
			hWNd = hWndParent;
	} while (hWndParent != NULL);
	return hWNd;
}

void GetAllProcessInfo()
{
	char szprid[MAX_PATH];
	memset(szprid, 0, MAX_PATH);
	HANDLE  hSnap = NULL;
	PROCESSENTRY32  pe = { sizeof(PROCESSENTRY32) };
	hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //创建快照
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		MessageBox(NULL, _T("创建快照失败"), _T(""), MB_OK);
	}

	if (!::Process32First(hSnap, &pe))
	{
		::CloseHandle(hSnap);
	}

	LPTSTR p = g_szExeName;
	TCHAR szExeName[512] = {};
	do {
		p++;
		ZeroMemory(szExeName, sizeof(szExeName));
		memcpy(szExeName, p, sizeof(szExeName) - 1);
		p = _tcsstr(p, _T("\\"));
	} while (p != NULL);

	do
	{
		if (_tcscmp(szExeName, pe.szExeFile) == 0)
		{
			return;
		}

	} while (::Process32Next(hSnap, &pe));
	::CloseHandle(hSnap);
}*/

BOOL CALLBACK EnumWindowsProc(
	_In_ HWND   hWnd,
	_In_ LPARAM lParam
)
{
	LPENUMWINDOWPARAM lpEmParam = (LPENUMWINDOWPARAM)lParam;
	TCHAR tszExeName[512] = {};
	GetExeName(hWnd, tszExeName, sizeof(tszExeName) - 1);
	if (_tcscmp(tszExeName, lpEmParam->tszExeName) == 0)
	{
		// if (FALSE == IsIconic(hWnd))
		//hWnd = _GetTopWindow(hWnd);
		TCHAR tszWindowClass[64] = {};
		TCHAR tszWindowName[64] = {};
		GetClassName(hWnd, tszWindowClass, sizeof(tszWindowClass) - 1);
		//GetWindowText(hWnd, tszWindowName, sizeof(tszWindowName) - 1);
		if (IsWindowVisible(hWnd)
			&& _tcscmp(tszWindowClass, lpEmParam->tszWindowClass) == 0
			//&& _tcscmp(tszWindowName, lpEmParam->tszWindowName) == 0
			)
			lpEmParam->hWnds[lpEmParam->dwIndex++] = hWnd;
	}
	return TRUE;
}

VOID OpenNewWindow()
{
	HWND hWndTop = GetForegroundWindow();
	TCHAR tszExePath[512] = {};
	GetExeName(hWndTop, tszExePath, sizeof(tszExePath) - 1);

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };
	CreateProcess(NULL, tszExePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
}

VOID SwitchWindow()
{
	LPENUMWINDOWPARAM lpEmParam = new PENUMWINDOWPARAM;
	ZeroMemory(lpEmParam, sizeof(PENUMWINDOWPARAM));
	HWND hWndTop = GetForegroundWindow();
	lpEmParam->hWnd = hWndTop;
	GetExeName(hWndTop, lpEmParam->tszExeName, sizeof(lpEmParam->tszExeName) - 1);
	GetClassName(hWndTop, lpEmParam->tszWindowClass, sizeof(lpEmParam->tszWindowClass) - 1);
	//GetWindowText(hWndTop, lpEmParam->tszWindowName, sizeof(lpEmParam->tszWindowName) - 1);
	lpEmParam->dwIndex = 0;

	//GetAllProcessInfo();
	EnumWindows(EnumWindowsProc, (LPARAM)lpEmParam);

	for (DWORD i = 1; i < lpEmParam->dwIndex; i++)
	{
		for (DWORD j = lpEmParam->dwIndex - 1; j >= i; j--)
		{
			if (lpEmParam->hWnds[j] < lpEmParam->hWnds[j - 1])
			{
				HWND a = lpEmParam->hWnds[j - 1];
				lpEmParam->hWnds[j - 1] = lpEmParam->hWnds[j];
				lpEmParam->hWnds[j] = a;
			}
		}
	}

	for (DWORD i = 0; i < lpEmParam->dwIndex; i++)
	{
		if (hWndTop == lpEmParam->hWnds[i])
		{
			HWND hWnd = lpEmParam->hWnds[i + 1];
			if (i == lpEmParam->dwIndex - 1)
			{
				hWnd = lpEmParam->hWnds[0];
			}
			
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetForegroundWindow(hWnd);
			break;
		}
	}
}