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
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void CreateNewShape(ShapeFactory*);
void ClearWindow();

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
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MAINMENU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_PAINT));

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
      return FALSE;
 
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
	const int numButtons = 8;
	const int iconSize = 16;

	DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndTB = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | CCS_TOP| TBSTYLE_FLAT|TBSTYLE_WRAPABLE , 0, 0, 0, 0,
		parent, NULL, hInst, NULL);
	if (hWndTB == NULL)
		return NULL;

	// Create the image list.
	HIMAGELIST hImgl = ImageList_Create(iconSize, iconSize,   // Dimensions of individual bitmaps.
		ILC_COLOR32 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);
	for (int i = 0; i < numButtons; i++)
		ImageList_AddIcon(hImgl, LoadIcon(hInst,MAKEINTRESOURCE(IDI_PENICON+i)));

	// Set the image list.
	SendMessage(hWndTB, TB_SETIMAGELIST,(WPARAM)ImageListID,(LPARAM)hImgl);

	// Load the button images.
	SendMessage(hWndTB, TB_LOADIMAGES,(WPARAM)IDB_STD_SMALL_COLOR,(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons];
	std::vector<std::string> buttonsText = {"Карандаш","Линия","Прямоугольник","Эллипс","Многоугольник","Текст","Заливка","Изменение цветов"};
	for (int i = 0; i < numButtons; i++)
	{
		tbButtons[i].iBitmap = i;
		tbButtons[i].fsStyle = buttonStyles;
		tbButtons[i].fsState = TBSTATE_ENABLED;
		tbButtons[i].iString = (INT_PTR)buttonsText[i].data();
		tbButtons[i].idCommand = IDM_PEN+i;
	}
	// Add buttons.
	SendMessage(hWndTB, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndTB, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndTB, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndTB, SW_SHOW);
	return hWndTB;
}

COLORREF stdColor = RGB(255,255,255);
Shape* currentShape;
ShapeFactory* currentFactory;
std::vector<Shape*> shapes;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,k;
	static POINTS startPoint,endPoint;
	static HPEN hPen;
	static CHOOSECOLOR ccs;
	static HBRUSH hBrush;
	static COLORREF acrCustColor[16];
	PAINTSTRUCT ps;
	HDC hdc, hdcEMF=NULL;
	static HENHMETAFILE hmf;
	static int sx, sy, currentShapeId=0,iVscrollPos, iHscrollPos, sizeToolbar;
	static HWND hWndToolBar;
	RECT rt;
	static TOOLINFO ti;
	static LOGFONT lf;
	static CHOOSEFONT cf;
	static HFONT hfont;
	static TCHAR name[256] = _T("");
	static OPENFILENAME file;
	std::ifstream in;
	std::ofstream out;
	static std::map <int, ShapeFactory*> shapeFactoryMap = { { IDM_LINE, new LineFactory }, { IDM_ELLIPSE, new EllipseFactory }, { IDM_RECTANGLE, new RectangleFactory } };
	std::vector <std::string>::iterator it;
	std::string st;
	switch (message)
	{
		case WM_CREATE:
			file.lStructSize = sizeof(OPENFILENAME);
			file.hwndOwner = hWnd;
			file.hInstance = hInst;
			file.lpstrFilter = _T("Metafile\0*.emf");
			file.lpstrFile = name;
			file.nMaxFile = 256;
			file.lpstrInitialDir = _T(".\\");
			file.lpstrDefExt = _T("emf");
			hWndToolBar = CreateToolBar(hWnd, hInst);
			hdcEMF = CreateEnhMetaFile(NULL, NULL, NULL, NULL);
			hPen = CreatePen(PS_SOLID, 3, RGB(120, 0, 0));
			
			ccs.lStructSize = sizeof(CHOOSECOLOR);
			ccs.hwndOwner = hWnd;
			ccs.rgbResult = stdColor;
			ccs.Flags =NULL;
			ccs.lpCustColors = (LPDWORD)acrCustColor;
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
			if (wParam == SIZE_MINIMIZED)
				break;
			sx = LOWORD(lParam);
			sy = HIWORD(lParam);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
				case IDM_FILENEW:
					ClearWindow();
					InvalidateRect(hWnd,NULL,TRUE);
					break;
				case IDM_FILEOPEN:
					file.lpstrTitle = _T("Открыть файл для чтения");
					file.Flags = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
					if (!GetOpenFileName(&file))
						return 1;
					in.open(name);
//					while (getline(in, st))
//						v.push_back(st);
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
				case IDM_PEN:
				case IDM_LINE: 
				case IDM_ELLIPSE:
				case IDM_RECTANGLE:
				case IDM_POLYGON:
				case IDM_TEXT:currentShapeId = wmId;
					break;
				case IDM_CHOOSECOLOR:
					if (ChooseColor(&ccs))
					{
						stdColor = ccs.rgbResult;
						if (hBrush)
							DeleteObject(hBrush);
						hBrush = CreateSolidBrush(stdColor);
					}
					break;
				case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:DestroyWindow(hWnd);
					break;
				default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			if (currentShapeId != 0)
			{
				startPoint =MAKEPOINTS(lParam);
				CreateNewShape(shapeFactoryMap.find(currentShapeId)->second);
				currentShape->SetPoint(startPoint);
				currentShape->SetPoint(startPoint);
			}
			break;
		case WM_MOUSEMOVE:
			if (wParam == MK_LBUTTON && currentShapeId!=0)
			{
				endPoint = MAKEPOINTS(lParam);
				currentShape->SetPoint(endPoint);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_LBUTTONUP:	
			break;
		case WM_PAINT:
		{
            if (currentShape != NULL)
			{
				int clientRectTop = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION);
				GetClientRect(hWnd, &rt);
				hdc = GetDC(hWnd);
				HDC hdc2 = CreateCompatibleDC(hdc);
				HBITMAP hbm = CreateCompatibleBitmap(hdc, sx, sy);
				HANDLE hold = SelectObject(hdc2, hbm);
				SelectObject(hdc2, hPen);
				FillRect(hdc2, &rt, (HBRUSH)WHITE_BRUSH);
				if (!shapes.empty())
					for (int i = 0; i < shapes.size(); i++)
						shapes[i]->Draw(hdc2, hPen);
				BitBlt(hdc, 0, clientRectTop, sx, sy - clientRectTop, hdc2, 0, clientRectTop, SRCCOPY);
				SelectObject(hdc2, hold);
				ReleaseDC(hWnd,hdc);
				DeleteObject(hbm);
				DeleteDC(hdc2);
;				rt.top= clientRectTop;
				ValidateRect(hWnd,&rt);
			}
		}
		break;
		case WM_DESTROY:
			DeleteEnhMetaFile(hmf);
			PostQuitMessage(0);
			break;
		default:return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void ClearWindow()
{
	for (int i = 0; i < shapes.size(); i++)
		delete shapes[i];
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
}

void CreateNewShape(ShapeFactory* shapeFactory)
{
	currentShape = shapeFactory->CreateShape();
	currentFactory = shapeFactory;
	shapes.push_back(currentShape);
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
