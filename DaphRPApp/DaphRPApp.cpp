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

void PlayMouseEvent()
{
	std::ifstream fin;
	fin.open(L"rec.log", std::ios::in);
	std::string rec;
	int prevtime = 0;
	char* ctx = nullptr;
	while (true)
	{
		char line[128] = { 0 };
		fin >> line;
		if (line[0] == '\0')
			continue;

		char* token = strtok_s(line, ",", &ctx);
		if (token == nullptr)
			continue;

		int msg = atoi(token);

		token = strtok_s(nullptr, ",", &ctx);
		if (token == nullptr)
			continue;

		int x = atoi(token);

		token = strtok_s(nullptr, ",", &ctx);
		if (token == nullptr)
			continue;

		int y = atoi(token);

		token = strtok_s(nullptr, ",", &ctx);
		if (token == nullptr)
			continue;

		int timestamp = atoi(token);

		int time = prevtime == 0 ? 0 : timestamp - prevtime;
		if (time < 0)
			time = 0;

		INPUT inp[1] = { 0 };
		inp[0].type = INPUT_MOUSE;
		inp[0].mi.time = 0;
		inp[0].mi.dwExtraInfo = 0;
		inp[0].mi.dx = x * (65535 / GetSystemMetrics(SM_CXSCREEN));
		inp[0].mi.dy = y * (65535 / GetSystemMetrics(SM_CYSCREEN));
		inp[0].mi.mouseData = 0;
		inp[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

		switch (msg)
		{
		case WM_LBUTTONDOWN:
			inp[0].mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
			break;
		case WM_LBUTTONUP:
			inp[0].mi.dwFlags |= MOUSEEVENTF_LEFTUP;
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_MOUSEWHEEL:
			break;
		case WM_MOUSEHWHEEL:
			break;
		case WM_RBUTTONDOWN:
			inp[0].mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
			break;
		case WM_RBUTTONUP:
			inp[0].mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
			break;
		}

		Sleep(time);
		SendInput(1, inp, sizeof(INPUT));

		prevtime = timestamp;
	}
	fin.close();
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
		case IDC_PLAY_BTN:
			PlayMouseEvent();
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
