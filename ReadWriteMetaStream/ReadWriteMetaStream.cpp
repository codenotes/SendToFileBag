// ReadWriteMetaStream.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ReadWriteMetaStream.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


#include <windows.h>
#include <stdio.h>

void writestream()
{
	HANDLE hFile, hStream;
	DWORD dwRet;

	hFile = CreateFileA("c:\\temp\\testfile.test",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		printf("Cannot open testfile\n");
	else
		WriteFile(hFile, "This is testfile", 16, &dwRet, NULL);

	hStream = CreateFileA("c:\\temp\\testfile.test:stream",
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hStream == INVALID_HANDLE_VALUE)
		printf("Cannot open testfile:stream\n");
	else
		WriteFile(hStream, "This is testfile:stream", 23, &dwRet, NULL);
}

int
getStreamSize(LPCTSTR     lpFileName);

void readstream()
{
	auto hStream = CreateFileA("c:\\temp\\testfile.test:stream",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL);
	if (hStream == INVALID_HANDLE_VALUE)
		printf("Cannot open testfile:stream\n");
	else
	{
		DWORD dwRet, dwFlags=0;
		char temp[255];
		
		WIN32_FIND_STREAM_DATA findStreamData;

		auto hFindStream = FindFirstStreamW(L"c:\\temp\\testfile.test:stream",
			FindStreamInfoStandard,
			&findStreamData,
			dwFlags);

		FindClose(hFindStream);
		auto sz = findStreamData.StreamSize.LowPart;

		ReadFile(hStream,(void*)temp,sz,&dwRet, NULL);
		temp[sz ] = 0;
		MessageBoxA(0, temp, "!", MB_OK);
	}

}


int
getStreamSize(	LPCTSTR     lpFileName)
{
	LPVOID               lpFindStreamData;
	HANDLE               hFindStream;
	STREAM_INFO_LEVELS   InfoLevel;
	DWORD                dwFlags;
	HANDLE               hFile;


	lpFindStreamData = NULL;
	//InfoLevel = 0;
	dwFlags = 0;
	WIN32_FIND_STREAM_DATA findStreamData;

	hFindStream = FindFirstStreamW(lpFileName,
		FindStreamInfoStandard,
		&findStreamData,
		dwFlags);

	FindClose(hFindStream);
	
	return  findStreamData.StreamSize.LowPart;

	//if (!hFindStream == INVALID_HANDLE_VALUE)
	//{
	//	FindNextStreamW(hFindStream,
	//		lpFindStreamData);
	//}

	return 0;
}

#include <string>
#include <tuple>
#include "Winuser.h"
#include "..\dialogtemplate.h"

LRESULT CALLBACK dlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	std::pair<std::string, std::string>* pUserPass = NULL;

	if (Msg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hDlg, GWLP_USERDATA, lParam);
	}
	else
	{
		pUserPass = reinterpret_cast<std::pair<std::string, std::string>*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
		
	}

	switch (Msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			wchar_t szUserData[64];
			wchar_t szPassData[64];
			if (!GetDlgItemText(hDlg, 25, szUserData, 64))
				*szUserData = 0;
			if (!GetDlgItemText(hDlg, 26, szPassData, 64))
				*szPassData = 0;

			if (pUserPass && szUserData && szPassData)
			{
				std::wstring wuser(szUserData);
				std::wstring wpass(szPassData);
				pUserPass->first = std::string(wuser.begin(), wuser.end());
				pUserPass->second = std::string(wpass.begin(), wpass.end());
			}
			EndDialog(hDlg, IDOK);
			return 1;
		}
		break;
		case IDCANCEL:
		{
			EndDialog(hDlg, IDCANCEL);
			return 1;
		}
		break;
		default:
			break;
		}
		break;
	}
	case WM_CLOSE:
		EndDialog(hDlg, IDCANCEL);
		return 1;
	}

	return 0;
}


bool UserPassBox(std::pair<std::string, std::string> &UserPass)
{
	DialogTemplate dialogTemplate("Enter Tag", WS_CAPTION | DS_CENTER | WS_EX_TOPMOST, 0, 0, 149, 145, "Tahoma");
	dialogTemplate.AddStatic("Tag:", WS_VISIBLE, 0, 3, 3, 20, 10, -1);
//	dialogTemplate.AddStatic("PWD:", WS_VISIBLE, 0, 3, 16, 20, 10, -1);
	//dialogTemplate.AddEditBox("", WS_VISIBLE | WS_TABSTOP, WS_EX_STATICEDGE | ES_MULTILINE | WS_VSCROLL, 25, 3, 80, 40, 25);
	dialogTemplate.AddEditBox("", WS_BORDER | WS_VISIBLE | WS_TABSTOP |WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL |ES_WANTRETURN,
		0, 25, 3, 120, 65, 25); //WS_EX_LEFT, ES_LEFT
	
	//	dialogTemplate.AddEditBox("", WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, WS_EX_STATICEDGE, 25, 16, 80, 10, 26);
	dialogTemplate.AddButton("&Confirm", WS_VISIBLE | WS_TABSTOP, 0, 3, 129, 50, 13, IDOK);
	dialogTemplate.AddButton("&Exit", WS_VISIBLE | WS_TABSTOP, 0, 56, 129, 50, 13, IDCANCEL);
	if (DialogBoxIndirectParam(NULL, dialogTemplate, NULL, (DLGPROC)dlgProc, (LPARAM)&UserPass) != IDOK)
		return false;

	MessageBoxA(NULL, UserPass.first.c_str(), "User:", 0);        //remove these
	//MessageBoxA(NULL, UserPass.second.c_str(), "Password:", 0);

	return true;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	std::pair<std::string, std::string> UserPass;
	UserPassBox(UserPass);
	return 0;
	
	//readstream();
    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_READWRITEMETASTREAM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_READWRITEMETASTREAM));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_READWRITEMETASTREAM));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_READWRITEMETASTREAM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
