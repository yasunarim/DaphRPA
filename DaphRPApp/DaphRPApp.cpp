// DaphRPApp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "DaphRPApp.h"
#include "resource.h"
#include "DaphRPAHook.h"

LRESULT CALLBACK DialogProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPWSTR    lpCmdLine,
						_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DAPH_DIALOG), NULL, (DLGPROC)DialogProc);
	return (int) 1;
}

void SaveMouseEvent(DWORD message, LPMSLLHOOKSTRUCT mousell)
{
	std::ofstream fout;
	fout.open(L"rec.log", std::ios::app);
	fout << message << "," << mousell->pt.x << "," << mousell->pt.y << "," << mousell->time << std::endl;
	fout.close();
}

LRESULT CALLBACK DialogProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
{

	LPMSLLHOOKSTRUCT mousell = nullptr;

	switch (msg) {
	case WM_INITDIALOG:
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wp)) 
		{
		case IDC_REC_BTN:
			StartMouseHook(hDlgWnd);
			return FALSE;
		default:
			return FALSE;
		}
	case DAPHRPA_WM_MOUSEHOOK:
		mousell = (LPMSLLHOOKSTRUCT)lp;
		SaveMouseEvent(wp, mousell);
		delete mousell;
		return FALSE;
	default:
		return FALSE;
	}
	return TRUE;
}
