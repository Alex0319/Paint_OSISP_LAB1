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

std::vector<Shape*> shapes;
int weight=DEFAULT_WEIGHT;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	WeightChangeDialog(HWND, UINT, WPARAM, LPARAM);

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
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAINMENU));

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
	const int numButtons = 10;
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
	std::vector<std::string> buttonsText = {"Карандаш","Линия","Прямоугольник","Эллипс","Многоугольник","Ломаная","Текст","Заливка","Толщина","Изменение цветов"};
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
	ShowWindow(hWndTB, SW_SHOW);
	return hWndTB;
}

HWND CreateTrackBar(HWND hwndDlg, HINSTANCE hInst)
{
	InitCommonControls();
	HWND hWndTrackBar = CreateWindowEx(0,TRACKBAR_CLASS,"Trackbar Control",	WS_CHILD|WS_VISIBLE|TBS_AUTOTICKS|TBS_ENABLESELRANGE|TBS_TRANSPARENTBKGND,10, 20,200, 30,hwndDlg,NULL,hInst,	NULL);
	if (hWndTrackBar == NULL)
		return NULL;
	SendMessage(hWndTrackBar, TBM_SETRANGE, 0, (LPARAM)MAKELONG(0, DEFAULT_TRACKBAR_MAXSIZE));
	SendMessage(hWndTrackBar, TBM_SETPAGESIZE,0, (LPARAM)DEFAULT_TRACKBAR_PAGESIZE);                  // new page size 
	SendMessage(hWndTrackBar, TBM_SETSEL, (WPARAM)FALSE, (LPARAM)MAKELONG(0, DEFAULT_TRACKBAR_MAXSIZE)); // redraw flag
	SendMessage(hWndTrackBar, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)weight);                   // redraw flag
	SetFocus(hWndTrackBar);
	return hWndTrackBar;
}

void InitOpenFileStruct(HWND hWnd,OPENFILENAME* file, TCHAR* name)
{
	file->lStructSize = sizeof(OPENFILENAME);
	file->hwndOwner = hWnd;
	file->hInstance = hInst;
	file->lpstrFilter = _T("Metafile\0*.emf");
	file->lpstrFile = name;
	file->nMaxFile = 256;
	file->lpstrInitialDir = _T(".\\");
	file->lpstrDefExt = _T("emf");
}

void ResetPolygonValues(POINTS* startPoint, POINTS* endPoint, bool* isCreatePolygon)
{
	startPoint->x = 0;
	startPoint->y = 0;
	endPoint->x = 0;
	endPoint->y = 0;
	*isCreatePolygon = false;
}

HENHMETAFILE OpenEnhMetaFile(OPENFILENAME* file,char* name)
{
	file->lpstrTitle = _T("Открыть файл для чтения");
	file->Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (!GetOpenFileName(file))
		return NULL;
	return GetEnhMetaFile(name);
}

void ResetAll(HWND hWnd,bool* isFileChanged,int* currentShapeId)
{
	for (int i = 0; i < shapes.size(); i++)
		delete shapes[i];
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
	*isFileChanged = false;
	*currentShapeId = 0;
	InvalidateRect(hWnd, NULL, TRUE);
}

Shape* CreateNewShape(ShapeFactory* shapeFactory, COLORREF color,int weight,COLORREF brushColor)
{
	Shape* currentShape = shapeFactory->CreateShape(color,weight,brushColor);
	shapes.push_back(currentShape);
	return currentShape;
}

void DrawShapes(HDC hdc,int weight)
{
	if (!shapes.empty())
		for (int i = 0; i < shapes.size(); i++)
			shapes[i]->Draw(hdc, weight);
}

bool ShowSaveMessageBox(HWND hWnd, FileName* filename,bool isFileChanged)
{
	if (isFileChanged)
		switch (MessageBox(hWnd, filename->GetSaveQuestionString(), "Paint", MB_YESNOCANCEL))
		{
			case IDYES: SendMessage(hWnd, WM_COMMAND, IDM_FILESAVE, 0);
			case IDNO:return true;
			case IDCANCEL:return false;
		}
	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent,k;
	static POINTS startPoint,endPoint;
	static bool isCreatePolygon = false,isUpdateWindow=false,isFileChanged=false;
	static COLORREF stdColor = RGB(0, 0, 0);
	static COLORREF stdBrushColor = RGB(255,255,255);
	static Shape* currentShape;
	static FileName* filename;
	static CHOOSECOLOR ccs[2];
	static COLORREF acrCustColor[16];
	PAINTSTRUCT ps;
	HDC hdc;
	static HENHMETAFILE hmf;
	static int sx, sy, currentShapeId=0,iVscrollPos, iHscrollPos, sizeToolbar;
	static HWND hWndToolBar;
	RECT rt;
	static TCHAR name[256] = _T("");
	static OPENFILENAME file;
	static std::map <int, ShapeFactory*> shapeFactoryMap = { { IDM_LINE, new LineFactory }, { IDM_ELLIPSE, new EllipseFactory }, { IDM_RECTANGLE, new RectangleFactory }, { IDM_POLYGON, new PolygonFactory }, { IDM_POLYGONLINE, new PolygonLineFactory }, { IDM_PEN, new PenFactory }, {IDM_TEXT,new TextFactory} };
	const int clientRectTop = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION);

	switch (message)
	{
		case WM_CREATE:
			InitOpenFileStruct(hWnd,&file, name);
			hWndToolBar = CreateToolBar(hWnd, hInst);
			filename = new FileName();
			for (int i = 0; i < 2; i++)
			{
				ccs[i].lStructSize = sizeof(CHOOSECOLOR);
				ccs[i].hwndOwner = hWnd;
				ccs[i].Flags = CC_RGBINIT;
				ccs[i].lpCustColors = (LPDWORD)acrCustColor;
			}
			ccs[0].rgbResult = stdBrushColor;
			ccs[1].rgbResult = stdColor;
			filename->SetWindowCaption(hWnd);
			break;
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
				break;
			SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
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
					if (ShowSaveMessageBox(hWnd, filename, isFileChanged))
					{
						ResetAll(hWnd, &isFileChanged, &currentShapeId);
						filename->ResetFileName();
						filename->SetWindowCaption(hWnd);
						_strset_s(name, sizeof(name), '\0');
					}
					break;
				case IDM_FILEOPEN:
					if (ShowSaveMessageBox(hWnd, filename, isFileChanged))
					{
						if (hmf != NULL)
							DeleteEnhMetaFile(hmf);
						if ((hmf = OpenEnhMetaFile(&file, name)) == NULL)
							break;
						filename->SetFileName(name);
						filename->SetWindowCaption(hWnd);
						ResetAll(hWnd, &isFileChanged, &currentShapeId);
					}
					break;
				case IDM_FILESAVE:
					if (isFileChanged)
					{
						if (strlen(name) == 0)
						{
							file.lpstrTitle = _T("Открыть файл для записи");
							file.Flags = OFN_OVERWRITEPROMPT;
							if (!GetSaveFileName(&file))
							    return 1;
						}
						filename->SetFileName(name);
						filename->SetWindowCaption(hWnd);
						GetClientRect(hWnd, &rt);
						rt.top += clientRectTop;
						char tempName[_MAX_PATH];
						strcpy_s(tempName,sizeof(tempName),filename->CreateTempFileName(name));
						HDC newHdcEMF = CreateEnhMetaFile(NULL,tempName ,NULL, NULL);
						FillRect(newHdcEMF,&rt, (HBRUSH)WHITE_BRUSH);
						PlayEnhMetaFile(newHdcEMF, hmf, &rt);
						DrawShapes(newHdcEMF, weight);
						HENHMETAFILE hmf2 = CloseEnhMetaFile(newHdcEMF);
						DeleteEnhMetaFile(hmf);
						DeleteEnhMetaFile(hmf2);
						remove(name);
						rename(tempName, name);
						isFileChanged = false;
						hmf = GetEnhMetaFile(name);
					}
					break;
				case IDM_PEN:
				case IDM_LINE: 
				case IDM_ELLIPSE:
				case IDM_RECTANGLE:
				case IDM_POLYGON:
				case IDM_POLYGONLINE:
				case IDM_TEXT:
					currentShapeId = wmId;
					currentShape = NULL;
					ResetPolygonValues(&startPoint, &endPoint, &isCreatePolygon);
					break;
				case IDM_PAINTCAN:
					if (ChooseColor(&ccs[0]))
						stdBrushColor = ccs[0].rgbResult;
					break;
				case IDM_CHOOSECOLOR:
					if (ChooseColor(&ccs[1]))
						stdColor = ccs[1].rgbResult;
					break;
				case IDM_WEIGHT:DialogBox(hInst, MAKEINTRESOURCE(IDD_WEIGHTCHANGEBOX), hWnd, WeightChangeDialog);
					break;
				case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			if (currentShapeId != 0)
			{
				isFileChanged = true;
				if (!isCreatePolygon)
				{
					currentShape = CreateNewShape(shapeFactoryMap.find(currentShapeId)->second, stdColor,weight,stdBrushColor);
					currentShape->SetPoint(MAKEPOINTS(lParam));
				}
				if (currentShapeId == IDM_POLYGON || currentShapeId==IDM_POLYGONLINE)
				{
					endPoint = MAKEPOINTS(lParam);
					if (startPoint.x == 0 && startPoint.y == 0)
						startPoint = endPoint;
					if (!isCreatePolygon)
						isCreatePolygon = true;
					InvalidateRect(hWnd, NULL, FALSE);
				}
			}
			break;
		case WM_MOUSEMOVE:
			if (wParam == MK_LBUTTON && currentShapeId!=0 && currentShapeId!=IDM_TEXT)
			{
				if (!isCreatePolygon)
					currentShape->SetPoint(MAKEPOINTS(lParam));
				else
					endPoint = MAKEPOINTS(lParam);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_LBUTTONUP:
			if (currentShapeId != 0 && isCreatePolygon)
			{
				startPoint = MAKEPOINTS(lParam);
				currentShape->SetPoint(startPoint);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_RBUTTONDOWN:ResetPolygonValues(&startPoint, &endPoint, &isCreatePolygon);
			break;
		case WM_MBUTTONDOWN:startPoint = MAKEPOINTS(lParam);
			break;
		case WM_MBUTTONM
		case WM_CHAR:
			if (currentShape!=NULL && currentShapeId == IDM_TEXT)
			{
				Text* temp = dynamic_cast<Text*>(currentShape);
				temp->SetChar(wParam);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_SYSCOMMAND:
			isUpdateWindow = true;
			return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_PAINT:
		{
			GetClientRect(hWnd, &rt);
			hdc = GetDC(hWnd);
			HDC hdc2 = CreateCompatibleDC(hdc);
			HBITMAP hbm = CreateCompatibleBitmap(hdc, sx, sy);
			HANDLE hold = SelectObject(hdc2, hbm);
			rt.top += clientRectTop;
			FillRect(hdc2, &rt, (HBRUSH)WHITE_BRUSH);
			if (hmf != NULL)
				PlayEnhMetaFile(hdc2, hmf, &rt);
			SetDCPenColor(hdc2, stdColor);
			SetDCBrushColor(hdc2,stdBrushColor);
			DrawShapes(hdc2, weight);
			if (isCreatePolygon)
			{
				MoveToEx(hdc2, startPoint.x, startPoint.y, NULL);
				LineTo(hdc2, endPoint.x, endPoint.y);
			}
			BitBlt(hdc, 0, clientRectTop, sx, sy - clientRectTop, hdc2, 0, clientRectTop, SRCCOPY);
			SelectObject(hdc2, hold);
			ReleaseDC(hWnd,hdc);
			DeleteObject(hbm);
			DeleteDC(hdc2);
			ValidateRect(hWnd, &rt);
			if (isUpdateWindow)
			{
				UpdateWindow(hWndToolBar);
				isUpdateWindow = false;
			}
		}
		break;
		case WM_CLOSE:
			if (isFileChanged)
				switch (MessageBox(hWnd, filename->GetSaveQuestionString(), "Paint", MB_YESNOCANCEL))
				{
					case IDYES:SendMessage(hWnd, WM_COMMAND, IDM_FILESAVE, 0);
					case IDNO:DestroyWindow(hWnd);
						break;
					case IDCANCEL:
						break;
				}
			else
				DestroyWindow(hWnd);
			break;
		case WM_DESTROY:
			DeleteEnhMetaFile(hmf);
			free(filename);
			PostQuitMessage(0);
			break;
		default:return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK WeightChangeDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hWndTrackBar=NULL;
	PAINTSTRUCT ps;
	static char currentPosString[64] = {""};
	static int currentPos=weight;
	switch (message)
	{
		case WM_INITDIALOG:	
			hWndTrackBar = CreateTrackBar(hDlg, hInst);
			if (currentPos != weight)
				currentPos = weight;
			InvalidateRect(hDlg, NULL, FALSE);
			break;
		case WM_HSCROLL:
			switch (LOWORD(wParam))
			{
				case TB_LINEUP:
					if (currentPos>0)
						currentPos -= 1;
					break;
				case TB_LINEDOWN:
					if (currentPos<DEFAULT_TRACKBAR_MAXSIZE)
						currentPos +=1;
					break;
				case TB_PAGEUP:
					if (currentPos>=0)
						currentPos -= DEFAULT_TRACKBAR_PAGESIZE;
					break;
				case TB_PAGEDOWN:
					if (currentPos<DEFAULT_TRACKBAR_MAXSIZE)
						currentPos += DEFAULT_TRACKBAR_PAGESIZE;
					break;
				case TB_THUMBTRACK:
				case TB_THUMBPOSITION:currentPos = HIWORD(wParam);
					break;
				case TB_TOP:currentPos =0;
					break;
				case TB_BOTTOM:currentPos = DEFAULT_TRACKBAR_MAXSIZE;
					break;
				break;
			}
			InvalidateRect(hDlg, NULL, FALSE);
			break;
		case WM_PAINT:
			{
				HDC hdc = BeginPaint(hDlg, &ps);
				wsprintf(currentPosString, " Толщина: %d  ", currentPos);
				TabbedTextOut(hdc, 65, 5, currentPosString, strlen(currentPosString), 0, 0, 0);
				EndPaint(hDlg, &ps);
			}
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK)
				weight = currentPos;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
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
