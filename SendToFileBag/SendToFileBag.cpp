// SendToFileBag.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SendToFileBag.h"


namespace fs = std::experimental::filesystem;

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
using namespace std;


int ItemStartIndex = 1;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

//D:\Epic Games\4.13\Engine\Source\ThirdParty\python
	//string f = "D:\\Epic Games\\4.13\\Engine\\Source\\ThirdParty\\python";
	//std::size_t found = f.find_last_of("/\\");
	//auto lastdir = f.substr(found + 1);
	////if(isFile("D:\\Epic Games\\4.13\\Engine\\Source\\ThirdParty\\python\\"))
	//if(dirOrFileExists("D:\\Epic Games\\4.13\\Engine\\Source\\ThirdParty\\python\\"))
	//{
	//	printf("ys");
	//}
	//else
	//{
	//	printf("no");
	//}

    // TODO: Place code here.
	std::wstring ws; 
	std::string dumpdir;

	//auto s = GetClipboardText();
	//auto b = dirOrFileExists(s);
	//char temp[255];
	//sprintf(temp, "text:%s isDirOrFile:%d", s.c_str(), b);
	//MessageBoxA(0, temp, "!", MB_OK);

	if (__argc <= 1)
		return 0;

	auto first = ws2s(__targv[1]);

	if (  (first == "/ClearClipboard") || (first=="/ClearClipboardAndPaste")  )
	{
		if (OpenClipboard(NULL))
		{
			EmptyClipboard();
			CloseClipboard();
		}
		//setClipBoardString("");
		if(first == "/ClearClipboard")
			return 0;

		ItemStartIndex = 2; //because we don't want to treat the command line flag as if it where a path
	}
	

	try
	{
		char *c=getenv("SEND_TO_DUMP_LOCATION");
		if (c)
		{
			dumpdir = c;
		}
		else
		{

			dumpdir = "c:\\temp";
		//	MessageBoxA(0, "not found envar SEND_TO_DUMP_LOCATION","Error", MB_OK);
		}

	}
	catch (const std::exception&e)
	{
		MessageBoxA(0, "!",e.what(), MB_OK);
		return 0;
	}
	
	std::string cs, sall;
	StringToWString(ws, dumpdir);
	//MessageBoxW(0, ws.data(), lpCmdLine, MB_OK);

	fs::path dir(dumpdir);
	fs::path file("sendToDumpFile.txt");

	std::ofstream of(dir / file, ios_base::app);
	//cs=ws2s(lpCmdLine);
	//std::replace(cs.begin(), cs.end(),' ', '\n');
	for (int i = ItemStartIndex; i < __argc; i++)
	{
		cs = ws2s(__targv[i]);
		sall = sall + cs+"\r\n";
		of<<cs<<endl;
	}
	
	auto lastPaths = GetClipboardText();
	//MessageBoxA(0, lastPaths.c_str(), "Error", MB_OK);
	lastPaths += sall;
	setClipBoardString(lastPaths);








	return 0;
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SENDTOFILEBAG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SENDTOFILEBAG));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SENDTOFILEBAG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SENDTOFILEBAG);
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
