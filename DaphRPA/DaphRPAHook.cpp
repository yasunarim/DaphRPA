#include "pch.h"
#include "DaphRPAHook.h"

// マウスフックのコールバック関数
LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam, LPARAM lParam);
// フックハンドルを保持する変数
HHOOK g_hhook = nullptr;
// 呼び出しもとウィンドウのハンドルを保持する変数
HWND g_hparent = nullptr;

/// マウスフックを開始する
void __cdecl StartMouseHook(HWND parent)
{
	if (!g_hhook)
	{
		g_hparent = parent;
		g_hhook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, nullptr, 0);
	}
}

void __cdecl EndMouseHook()
{
	if (g_hhook)
		UnhookWindowsHookEx(g_hhook);
	g_hhook = nullptr;
	g_hparent = nullptr;
}


// マウスフックコールバックプロシージャ
LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (!g_hparent || code < 0)
		return CallNextHookEx(g_hhook, code, wParam, lParam);

	PostMessage(g_hparent, DAPHRPA_WM_MOUSEHOOK, wParam, lParam);

	return CallNextHookEx(g_hhook, code, wParam, lParam);
}
