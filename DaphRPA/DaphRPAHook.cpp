#include "pch.h"
#include "DaphRPAHook.h"

// �}�E�X�t�b�N�̃R�[���o�b�N�֐�
LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam, LPARAM lParam);
// �t�b�N�n���h����ێ�����ϐ�
HHOOK g_hhook = nullptr;
// �Ăяo�����ƃE�B���h�E�̃n���h����ێ�����ϐ�
HWND g_hparent = nullptr;

/// �}�E�X�t�b�N���J�n����
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


// �}�E�X�t�b�N�R�[���o�b�N�v���V�[�W��
LRESULT CALLBACK MouseHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (!g_hparent || code < 0)
		return CallNextHookEx(g_hhook, code, wParam, lParam);

	PostMessage(g_hparent, DAPHRPA_WM_MOUSEHOOK, wParam, lParam);

	return CallNextHookEx(g_hhook, code, wParam, lParam);
}
