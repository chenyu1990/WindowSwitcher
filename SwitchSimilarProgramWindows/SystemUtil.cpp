#include "SystemUtil.h"

VOID DebugPrivilege()
{
	HANDLE hToken;
	LUID Luid;
	TOKEN_PRIVILEGES tp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) return;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid))
	{
		CloseHandle(hToken);
		return;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = Luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, false, &tp, sizeof(tp), NULL, NULL))
	{
		CloseHandle(hToken);
		return;
	}
	CloseHandle(hToken);
}
