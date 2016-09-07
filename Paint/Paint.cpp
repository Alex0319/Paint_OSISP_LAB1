// Paint.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Paint.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:

HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
TCHAR szChildClass[] = _T("WinChild");


// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndChildProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: разместите код здесь.
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MAINMENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_PAINT));

	return RegisterClassEx(&wcex);
}

ATOM MyRegisterChildClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex ;
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.lpfnWndProc = WndChildProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = szChildClass;
	return RegisterClassEx(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//

HWND CreateToolBar(HWND parent,HINSTANCE hInst)
{
	const int ImageListID = 0;
	const int numButtons = 2;
	const int iconSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndTB = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_TOOLTIPS|CCS_TOP|WS_EX_CLIENTEDGE , 0, 0, 0, 0,
		parent, NULL, hInst, NULL);
	if (hWndTB == NULL)
		return NULL;

	// Create the image list.
	HIMAGELIST hImgl = ImageList_Create(iconSize, iconSize,   // Dimensions of individual bitmaps.
		ILC_COLOR32 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);

	ImageList_AddIcon(hImgl, LoadIcon(hInst,MAKEINTRESOURCE(IDI_PENICON)));
	ImageList_AddIcon(hImgl, LoadIcon(hInst,MAKEINTRESOURCE(IDI_LINEICON)));

	// Set the image list.
	SendMessage(hWndTB, TB_SETIMAGELIST,(WPARAM)ImageListID,(LPARAM)hImgl);

	// Load the button images.
	SendMessage(hWndTB, TB_LOADIMAGES,(WPARAM)IDB_STD_SMALL_COLOR,(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons];
	for (int i = 0; i < 2; i++)
	{
		tbButtons[i].iBitmap = i;
		tbButtons[i].fsStyle = buttonStyles;
		tbButtons[i].fsState = TBSTATE_ENABLED;
		tbButtons[i].iString = NULL;
		tbButtons->idCommand = 0;
	}
	// Add buttons.
	SendMessage(hWndTB, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndTB, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndTB, TB_AUTOSIZE, 0, 0);
	return hWndTB;
}


static HWND hWndChild;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,y,k;
	PAINTSTRUCT ps;
	HDC hdc, hdcEMF=NULL;
	static HENHMETAFILE hmf;
	static int sx, sy,iVscrollPos,iHscrollPos,sizeToolbar;
	static HWND hWndToolBar,hWndToolTip;
	RECT rt;
	static TOOLINFO ti;
	static LOGFONT lf;
	static CHOOSEFONT cf;
	static HFONT hfont;
	static TCHAR name[256] = _T("");
	static OPENFILENAME file;
	std::ifstream in;
	std::ofstream out;
	static std::vector<std::string> v;
	std::vector <std::string>::iterator it;
	std::string st;
	switch (message)
	{
		case WM_CREATE:
			file.lStructSize = sizeof(OPENFILENAME);
			file.hwndOwner = hWnd;
			file.hInstance = hInst;
			file.lpstrFilter = _T("Metafile\0*.wmf");
			file.lpstrFile = name;
			file.nMaxFile = 256;
			file.lpstrInitialDir = _T(".\\");
			file.lpstrDefExt = _T("wmf");
			hWndToolBar = CreateToolBar(hWnd, hInst);
			MyRegisterChildClass(hInst);
			hWndChild = CreateWindow(szChildClass,NULL,WS_CHILD|WS_VISIBLE|WS_BORDER|WS_SIZEBOX,CW_USEDEFAULT,0,CW_USEDEFAULT,0,hWnd,NULL,hInst,NULL);
			hdcEMF = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
			/*			cf.lStructSize = sizeof(CHOOSEFONT);
			cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;*/
			break;
/*		case WM_NOTIFY:
			if (((LPNMHDR)lParam)->code == TTN_GETDISPINFO)
			{
				char* str = "";
				LPNMTTDISPINFO pInfo = (LPNMTTDISPINFO)lParam;
//				wmId = GetDlgCtrlID(((LPNMHDR)lParam)->hwndFrom);
				SendMessage(pInfo->hdr.hwndFrom, TTM_SETMAXTIPWIDTH, 0, 150);
				strcpy_s(pInfo->szText,str);
			}
			break;*/
		case WM_SIZE:
			SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
			if (wParam == SIZE_MINIMIZED)
				break;
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
			MoveWindow(hWndChild, 20, GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION), sx - 40, sy - 40, TRUE);
			ShowWindow(hWndChild, SW_SHOW);
			UpdateWindow(hWndChild);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
			case IDM_MAIN:	ShowWindow(hWndToolBar, SW_SHOW);
					break;
				case IDM_FILENEW:
					if (!v.empty())
						std::vector<std::string>().swap(v);
					InvalidateRect(hWnd,NULL,TRUE);
					break;
				case IDM_FILEOPEN:
					file.lpstrTitle = _T("Открыть файл для чтения");
					file.Flags = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
					if (!GetOpenFileName(&file))
						return 1;
					in.open(name);
					while (getline(in, st))
						v.push_back(st);
					in.close();
					InvalidateRect(hWnd, NULL, 1);
					break;
				case IDM_FILESAVE:
				{
					 file.lpstrTitle = _T("Открыть файл для записи");
					 file.Flags = OFN_OVERWRITEPROMPT;
					 if (!GetSaveFileName(&file))
						 return 1;
					 HDC tempHdcEmf = CreateMetaFile(name);
					 memcpy_s(tempHdcEmf, sizeof(HDC), hdcEMF, sizeof(HDC));
					 HENHMETAFILE tempHmf = CloseEnhMetaFile(tempHdcEmf);
					 DeleteEnhMetaFile(tempHmf);
					 /*					out.open(name);
					 for (it = v.begin(); it != v.end(); it++)
					 out << *it << '\n';
					 out.close();*/
				}
				break;
				case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:DestroyWindow(hWnd);
					break;
				default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			for (y = 20 , it = v.begin(); it < v.end(); it++, y += 16)
				TabbedTextOutA(hdc, 0, y, it->data(), it->length(), 0, NULL, 0);
			// TODO: добавьте любой код отрисовки...
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			DeleteEnhMetaFile(hmf);
			PostQuitMessage(0);
			break;
		default:return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	return 0;
}

// Обработчик сообщений для окна "О программе".
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
