#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <commctrl.h>
#include "resource.h"

HINSTANCE hInstance = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Greet(HWND hWnd);

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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ID_APP_ICON));
	if(!hIcon) hIcon = LoadIcon(hInst, MAKEINTRESOURCE(ID_APP_ICON_ALT));
	wc.hIcon = hIcon ? hIcon : LoadIcon(NULL, IDI_INFORMATION);
	if(!RegisterClass(&wc)) return 1;
	// create window
	HWND window = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 440, 140, NULL, NULL, hInst, 0);
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

typedef struct _WindowData
{
	HWND hWnd;
	HFONT hFontMain;
	HWND hStatic;
	HWND hEdit;
	HWND hButton;
	RECT edit_rc;
	RECT static_rc;
	RECT button_rc;
} WindowData;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_CREATE:
			{
				WindowData *window_data = (WindowData*)malloc(sizeof(WindowData));
				if(window_data)
				{
					ZeroMemory(window_data, sizeof(WindowData));
					window_data->hWnd = hWnd;
					window_data->hFontMain = CreateFont(14, 7, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));
					RECT edit_rc = {0, 0, 180, 20};
					RECT static_rc = {edit_rc.left-edit_rc.right/2-30, 0, 50, 20};
					RECT button_rc = {edit_rc.left+edit_rc.right/2+55, 0, 100, 20};
					window_data->edit_rc = edit_rc;
					window_data->static_rc = static_rc;
					window_data->button_rc = button_rc;
					window_data->hStatic = CreateWindow(TEXT("STATIC"), TEXT("Name: "), WS_VISIBLE|WS_CHILD|SS_RIGHT, static_rc.left, static_rc.top, static_rc.right, static_rc.bottom, hWnd, (HMENU)0, hInstance, 0);
					window_data->hEdit = CreateWindow(TEXT("EDIT"), TEXT(""), WS_VISIBLE|WS_CHILD|ES_LEFT|ES_AUTOHSCROLL, edit_rc.left, edit_rc.top, edit_rc.right, edit_rc.bottom, hWnd, (HMENU)ID_EDIT_NAME, hInstance, 0);
					window_data->hButton = CreateWindow(TEXT("BUTTON"), TEXT("Call"), WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_DEFPUSHBUTTON|BS_CENTER, button_rc.left, button_rc.top, button_rc.right, button_rc.bottom, hWnd, (HMENU)ID_BUTTON_CALL, hInstance, 0);
					SendMessage(window_data->hStatic, WM_SETFONT, (WPARAM)window_data->hFontMain, 0);
					SendMessage(window_data->hEdit, WM_SETFONT, (WPARAM)window_data->hFontMain, 0);
					SendMessage(window_data->hButton, WM_SETFONT, (WPARAM)window_data->hFontMain, 0);
					SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window_data);
				}
			}
			break;
		case WM_SIZE:
			{
				SHORT width = (SHORT)LOWORD(lParam);
				SHORT height = (SHORT)HIWORD(lParam);
				WindowData *window_data = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
				if(window_data)
				{
					SetWindowPos(window_data->hStatic, NULL, ((width-window_data->static_rc.right)/2+window_data->static_rc.left), ((height-window_data->static_rc.bottom)/2+window_data->static_rc.top), 0, 0, SWP_NOSIZE);
					SetWindowPos(window_data->hEdit, NULL, ((width-window_data->edit_rc.right)/2+window_data->edit_rc.left), ((height-window_data->edit_rc.bottom)/2+window_data->edit_rc.top), 0, 0, SWP_NOSIZE);
					SetWindowPos(window_data->hButton, NULL, ((width-window_data->button_rc.right)/2+window_data->button_rc.left), ((height-window_data->button_rc.bottom)/2+window_data->button_rc.top), 0, 0, SWP_NOSIZE);
				}
			}
			break;
		case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
				lpMMI->ptMinTrackSize.x = 440;
				lpMMI->ptMinTrackSize.y = 140;
			}
			break;
		case WM_COMMAND:
			switch(wParam)
			{
				case ID_EDIT_NAME:
				case ID_BUTTON_CALL:
					Greet(hWnd);
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
			{
				WindowData *window_data = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
				if(window_data)
				{
					DestroyWindow(window_data->hStatic);
					window_data->hEdit = NULL;
					DestroyWindow(window_data->hEdit);
					window_data->hStatic = NULL;
					DestroyWindow(window_data->hButton);
					window_data->hFontMain = NULL;
					DeleteObject(window_data->hFontMain);
					window_data->hButton = NULL;
				}
			}
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

void Greet(HWND hWnd)
{
	{
		TCHAR message[512] = TEXT("Hello World!\n:)");
		WindowData *window_data = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(window_data)
		{
			const int edit_text_len = GetWindowTextLength(window_data->hEdit);
			TCHAR *edit_text = malloc((edit_text_len+1)*sizeof(TCHAR));
			if(edit_text)
			{
				GetWindowText(window_data->hEdit, edit_text, 128);
				int writen = _stprintf(message, "Hello %s!\n:D", edit_text);
				message[writen] = TEXT('\0');
			}
			free(edit_text);
		}
		MessageBox(hWnd, message, TEXT("Greetings!"), MB_OK);
	}
}
