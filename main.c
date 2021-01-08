#include <string.h>
#include <tchar.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

HINSTANCE hInstance = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE _hpInst, LPSTR szCmd, int nShow)
{
	hInstance = hInst;
	InitCommonControls();
	// register window
	const TCHAR *szClassName = TEXT("WINDOW_CLASS");
	TCHAR szTitle[128] = {TEXT('\0')};
	if(!LoadString(hInst, ID_APP_NAME, szTitle, 128)) _tcscpy(szTitle, TEXT("Window"));
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = szClassName;
	wc.lpszMenuName = MAKEINTRESOURCE(ID_MAIN_MENU);
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND+1;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ID_APP_ICON));
	if(!hIcon) hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ID_APP_ICON_ALT));
	wc.hIcon = hIcon ? hIcon : LoadIcon(NULL, IDI_INFORMATION);
	if(!RegisterClass(&wc)) return 1;
	// create window
	HWND window = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, 0);
	if(!window) return 1;
	// show window
	ShowWindow(window, nShow);
	// enter event loop
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			CreateWindow(TEXT("BUTTON"), TEXT("Button"), WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON|BS_CENTER, 100, 100, 100, 30, hWnd, (HMENU)ID_BUTTON_1, hInstance, 0);
			break;
		case WM_COMMAND:
			switch(wParam)
			{
				case ID_BUTTON_1:
					MessageBox(hWnd, TEXT("Hello World!\n:)"), TEXT("Greetings!"), MB_OKCANCEL);
					break;
				case ID_MAIN_MENU_ITEM_FILE_OPEN:
					MessageBox(hWnd, TEXT("Opening wormhole..."), TEXT("Open"), MB_OKCANCEL);
					break;
				case ID_MAIN_MENU_ITEM_FILE_EXIT:
					if(IDYES == MessageBox(hWnd, TEXT("Are you sure you want to quit?"), TEXT("Exitting..."), MB_YESNO|MB_ICONEXCLAMATION))
						DestroyWindow(hWnd);
					break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}