#pragma once
#ifdef DAPHRPA_EXPORTS
#define DAPHRPAAPI __declspec(dllexport)
#else
#define DAPHRPAAPI __declspec(dllimport)
#endif

#define DAPHRPA_WM_MOUSEHOOK        WM_APP + 21

extern "C"
{
	DAPHRPAAPI void __cdecl StartMouseHook(HWND parent);
	DAPHRPAAPI void __cdecl EndMouseHook();
}