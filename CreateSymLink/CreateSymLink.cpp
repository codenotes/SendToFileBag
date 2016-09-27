// CreateSymLink.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CreateSymLink.h"
// SendToFileBag.cpp : Defines the entry point for the application.
//



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





int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	std::wstring ws;
	std::string dumpdir;


	//if (__argc <= 1)
	//	return 0;
	//auto s = GetClipboardText();
	//auto b = dirOrFileExists(s);
	//char temp[255];
	//sprintf(temp, "text:%s isDirOrFile:%d", s.c_str(), b);
	//MessageBoxA(0, temp, "!", MB_OK);

	auto cb = GetClipboardText();
	auto files=split(cb.c_str(), '\n');

	if (files.size() == 0)
	{
		MessageBoxA(0, "No files or dirs on clipboard", "!", MB_OK);
		return 0;
	}
	//lets see if it is really a file
	//files[0] = '"' + files[0] + '"';
	if (!dirOrFileExists(strip(files[0])))
	{

		MessageBoxA(0, "No files or dirs on clipboard that actually exist", files[0].c_str(), MB_OK);
		return 0;
	}


	string executethis;
	string base;
	string lastdir;
	char fname[512];
	char temp[512];
	
	string destDir=ws2s(__targv[1]);

	for (auto f : files)
	{
		f = strip(f);
		strcpy(fname, f.c_str());
		PathStripPathA(fname);

		std::size_t found = f.find_last_of("/\\");

		if (!isFile(f))//found != string::npos) //found it, Junction to DIR
		{
			lastdir=f.substr(found+1);
			sprintf(temp, "create JUNCTION link:%s\nwith real path:%s\ndest:%s",
				lastdir.c_str(), //the directory name which is your link by itself
				f.c_str(), //the complete real path to the real directory
				destDir.c_str()); //the place you want to put lastDir as link by itself
			
			if (IDYES == MessageBoxA(0, temp, "!", MB_YESNOCANCEL))
			{
				fs::path dir(destDir);
				fs::path linkname(lastdir);
				auto thing = dir / linkname;
				auto b=CreateSymbolicLinkA(thing.string().c_str(),
					f.c_str(),
					SYMBOLIC_LINK_FLAG_DIRECTORY
				);

				if (!b)
				{
					auto error=GetLastErrorAsString();
					MessageBoxA(0, error.c_str(), "Error", MB_OK);

				}
			}

		}
		else if( dirOrFileExists(f)) //hard link to file
		{
			sprintf(temp, "create hard file link:\n%s\nwith real path:\n%s\ndest:\n%s",
				fname,  //simple link name (the file by itself)
				f.c_str(), //the complete real path to the file
				destDir.c_str()); //the place you want to put the fname link into
			if (IDYES == MessageBoxA(0, temp, "!", MB_YESNOCANCEL))
			{

				fs::path dir(destDir);
				fs::path linkname(fname);
				auto thing = dir / linkname;
				auto b = CreateSymbolicLinkA(thing.string().c_str(),
					f.c_str(),
					0 //means its a file
				);

				if (!b)
				{
					MessageBoxA(0, "That didn't work", "Error", MB_OK);
				}
			}

		
		}

		

	}




	return 0;

	try
	{
		char *c = getenv("SEND_TO_DUMP_LOCATION");
		if (c)
		{
			dumpdir = c;
		}
		else
		{
			MessageBoxA(0, "not found envar SEND_TO_DUMP_LOCATION", "Error", MB_OK);
		}

	}
	catch (const std::exception&e)
	{
		MessageBoxA(0, "!", e.what(), MB_OK);
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
	for (int i = 1; i < __argc; i++)
	{
		cs = ws2s(__targv[i]);
		sall = sall + cs + "\r\n";
		of << cs << endl;
	}


	setClipBoardString(sall);








	return 0;
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CREATESYMLINK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CREATESYMLINK));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_CREATESYMLINK));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CREATESYMLINK);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
