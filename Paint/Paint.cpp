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

const int clientRectTop = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXMENUSIZE);

int penColor, brushColor, weight = DEFAULT_WEIGHT;
float zoom = 1;
CHOOSECOLOR chooseColor[2];
OPENFILENAME file;
HWND hWndToolBar;
HCURSOR handCursor;
HCURSOR crossCursor;
Shape* currentShape;
TCHAR name[256] = _T("");
std::vector<Shape*> shapes;
std::map <int, ShapeFactory*> shapeFactoryMap = { { IDM_LINE, new LineFactory }, { IDM_ELLIPSE, new EllipseFactory }, { IDM_RECTANGLE, new RectangleFactory }, { IDM_POLYGON, new PolygonFactory }, { IDM_POLYGONLINE, new PolygonLineFactory }, { IDM_PEN, new PenFactory }, { IDM_TEXT, new TextFactory } };
FileName* filename;
HENHMETAFILE hmf;
POINT mousePos, prevOrigin, origin;
POINTS startPoint, endPoint;
bool isCreatePolygon, isFileChanged, isPrintSelected,isMoving;
int currentShapeId;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	WeightChangeDialog(HWND, UINT, WPARAM, LPARAM);

void SetOriginTo(POINT origin);

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
	handCursor = LoadCursor(NULL, IDC_HAND);
	crossCursor = LoadCursor(NULL, IDC_CROSS);

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	SetCursor(crossCursor);
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
	wcex.hCursor		= LoadCursor(hInst,IDC_ARROW);
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
	HWND hWndTB = CreateWindowEx(WS_EX_WINDOWEDGE, TOOLBARCLASSNAME, NULL,
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

void zoomIn(HWND hWnd,float currentZoom)
{
	if (((zoom * currentZoom) <= 8) && ((zoom * currentZoom) >= 0.25))
	{
		zoom *= currentZoom;
		InvalidateRect(hWnd, NULL, true);
	}
}

void MouseWheel(HWND hWnd, WPARAM wParam)
{
	WORD wParamLowWord = LOWORD(wParam);
	if ((wParamLowWord == MK_CONTROL) || (wParamLowWord == (MK_CONTROL | MK_SHIFT)))
	{
		WORD wheelDestiny = HIWORD(wParam);
		if (wheelDestiny > 65000)
			zoomIn(hWnd,0.5);
		else
			zoomIn(hWnd,2);
	}
}

void MouseMove(HWND hWnd, POINT prevMousePos, POINT currentMousePos)
{
	if((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		endPoint.x = currentMousePos.x;
		endPoint.y = currentMousePos.y;
		if (currentShapeId != 0 && !isCreatePolygon && currentShapeId != IDM_TEXT)
			currentShape->SetPoint(endPoint);
		InvalidateRect(hWnd, NULL, FALSE);
	}
	if (isMoving && (GetKeyState(VK_MBUTTON) & 0x100) != 0)
	{
		bool altPushed = (GetKeyState(VK_MENU) & 0x100) != 0;
		bool ctrlPushed = (GetKeyState(VK_CONTROL) & 0x100) != 0;
		bool shiftPushed = (GetKeyState(VK_SHIFT) & 0x100) != 0;
		if ((!shiftPushed && !altPushed && !ctrlPushed) || (shiftPushed && !altPushed && !ctrlPushed))
		{
			POINT prevOrigin = origin;
			POINT newOrigin;
			newOrigin.x = prevOrigin.x + (currentMousePos.x - prevMousePos.x);
			newOrigin.y = prevOrigin.y + (currentMousePos.y - prevMousePos.y);
			SetOriginTo(newOrigin);
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
}

void InitOpenFileStruct(HWND hWnd)
{
	file.lStructSize = sizeof(OPENFILENAME);
	file.hwndOwner = hWnd;
	file.hInstance = hInst;
	file.lpstrFilter = _T("Metafile\0*.emf");
	file.lpstrFile = name;
	file.nMaxFile = 256;
	file.lpstrInitialDir = _T(".\\");
	file.lpstrDefExt = _T("emf");
}

void InitChooseColorStruct(HWND hWnd)
{
	static COLORREF acrCustColor[16];
	for (int i = 0; i < 2; i++)
	{
		ZeroMemory(&chooseColor[i], sizeof(CHOOSECOLOR));
		chooseColor[i].lStructSize = sizeof(CHOOSECOLOR);
		chooseColor[i].hwndOwner = hWnd;
		chooseColor[i].Flags = CC_RGBINIT;
		chooseColor[i].lpCustColors = (LPDWORD)acrCustColor;
	}
	chooseColor[0].rgbResult = brushColor;
	chooseColor[1].rgbResult = penColor;
}

int ChooseColorProc(CHOOSECOLOR chooseColor)
{
	if (ChooseColor(&chooseColor))
		return chooseColor.rgbResult;
}

void InitResource(HWND hWnd)
{
	InitOpenFileStruct(hWnd);
	InitChooseColorStruct(hWnd);
	penColor = RGB(0, 0, 0);
	brushColor = RGB(255, 255, 255);
	hWndToolBar = CreateToolBar(hWnd, hInst);
	filename = new FileName();
	filename->SetWindowCaption(hWnd);
	currentShapeId = 0;
	origin.x = 0;
	origin.y = 0;
	prevOrigin.x = 0;
	prevOrigin.y = 0;
	isCreatePolygon = false;
	isFileChanged = false;
	isPrintSelected = false;
	isMoving = false;
}

void ResetPointValues(POINTS* startPoint, POINTS* endPoint)
{
	startPoint->x = 0;
	startPoint->y = 0;
	endPoint->x = 0;
	endPoint->y = 0;
}

HENHMETAFILE OpenEnhMetaFile(char* name)
{
	file.lpstrTitle = _T("Открыть файл для чтения");
	file.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	if (!GetOpenFileName(&file))
		return NULL;
	return GetEnhMetaFile(name);
}

Shape* CreateNewShape(ShapeFactory* shapeFactory)
{
	Shape* currentShape = shapeFactory->CreateShape(penColor, weight, brushColor);
	shapes.push_back(currentShape);
	return currentShape;
}

void DrawShapes(HDC hdc, int weight)
{
	if (!shapes.empty())
	for (int i = 0; i < shapes.size(); i++)
		shapes[i]->Draw(hdc, weight);
}

void RecalculateCoordinates(int maxWidth,int maxHeight)
{
	int shapesCount = shapes.size();
	int deltaX = origin.x - prevOrigin.x;
	int deltaY = origin.y - prevOrigin.y;
	startPoint.x += deltaX;
	startPoint.y += deltaY;
	endPoint.x += deltaX;
	endPoint.y += deltaY;
	for (int i = 0; i < shapesCount; i++)
		shapes[i]->RecalculateCoordinates(deltaX, deltaY,maxWidth,maxHeight);
}

void SetOriginTo(POINT newOrigin)
{
	prevOrigin = origin;
	origin = newOrigin;
}

POINTS SetNewPoint(POINTS point)
{
	point.x = point.x / zoom;
	point.y = (point.y -clientRectTop) / zoom;
	return point;
}

HENHMETAFILE SaveEnhanceMetafile(RECT rt,char* newName)
{
	char tempName[_MAX_PATH];
	strcpy_s(tempName, sizeof(tempName), newName);
	HDC newHdcEMF = CreateEnhMetaFile(NULL, tempName, NULL, NULL);
	FillRect(newHdcEMF, &rt, WHITE_BRUSH);
	if (hmf!=NULL)
		PlayEnhMetaFile(newHdcEMF, hmf, &rt);
	DrawShapes(newHdcEMF, weight);
	HENHMETAFILE hmf2 = CloseEnhMetaFile(newHdcEMF);
	DeleteEnhMetaFile(hmf2);
	if (hmf!=NULL)
		DeleteEnhMetaFile(hmf);
	remove(name);
	rename(tempName, name);
	return GetEnhMetaFile(name);
}

void PrintMetaFile(HWND hWnd, POINTS startPoint, POINTS endPoint, RECT windowRect)
{
	POINTS point;
	point.x = min(startPoint.x, endPoint.x);
	point.y = min(startPoint.y, endPoint.y) + clientRectTop+1;
	HDC tmpHDC = CreateEnhMetaFile(NULL, name, NULL, NULL);

	FillRect(tmpHDC, &windowRect, WHITE_BRUSH);
	BitBlt(tmpHDC, point.x + 1, point.y + 1, std::abs(endPoint.x - startPoint.x)- 2, std::abs(endPoint.y - startPoint.y) - 2, GetDC(hWnd), point.x + 1, point.y + 1, SRCCOPY);
	DeleteEnhMetaFile(CloseEnhMetaFile(tmpHDC));
}

void ClearShapesVector()
{
	for (int i = 0; i < shapes.size(); i++)
		delete shapes[i];
	if (!shapes.empty())
		std::vector<Shape*>().swap(shapes);
}

void ResetAll(HWND hWnd, int* currentShapeId,bool* isFileChanged)
{
	ClearShapesVector();
	*isFileChanged = false;
	*currentShapeId = 0;
	InvalidateRect(hWnd, NULL, TRUE);
}

bool ShowSaveMessageBox(HWND hWnd,bool isFileChanged)
{
	TCHAR messageText[256]=_T("");
	strcpy_s(messageText, sizeof(messageText), filename->GetSaveQuestionString());
	if (isFileChanged)
		switch (MessageBox(hWnd, _T(messageText),_T("Paint"), MB_YESNOCANCEL))
		{
			case IDYES: SendMessage(hWnd, WM_COMMAND, IDM_FILESAVE, 0);
			case IDNO:return true;
			case IDCANCEL:return false;
		}
	return true;
}

bool ShowSaveFileDialog(char* dialogCaption)
{
	file.lpstrTitle = _T(dialogCaption);
//	file.Flags = OFN_OVERWRITEPROMPT;
	if (!GetSaveFileName(&file))
		return false;
	return true;
}

void Drawing(HWND hWnd)
{
	RECT windowRect;
	GetClientRect(hWnd, &windowRect);
	windowRect.right = 1+windowRect.right / zoom;
	windowRect.bottom = 1+windowRect.bottom / zoom;
	int windowWidth = windowRect.right - windowRect.left;//!
	int windowHeight = windowRect.bottom - windowRect.top;//!
	int newClientRectTop = clientRectTop / zoom;
	HDC hdc = GetDC(hWnd);
	HDC hdc2 = CreateCompatibleDC(hdc);
	HBITMAP hbm = CreateCompatibleBitmap(hdc, windowWidth, windowHeight);
	HANDLE hold = SelectObject(hdc2, hbm);
	FillRect(hdc2, &windowRect, WHITE_BRUSH);
	if (hmf != NULL)
	{
		windowRect.right *= zoom;
		windowRect.bottom *= zoom;
		windowRect.left += origin.x;
		windowRect.right += origin.x;
		windowRect.bottom += origin.y;
		windowRect.top += origin.y;
		PlayEnhMetaFile(hdc2, hmf, &windowRect);
	}
	SetDCPenColor(hdc2, penColor);
	SetDCBrushColor(hdc2, brushColor);
	if (isMoving)
		RecalculateCoordinates(windowWidth,windowHeight);
	DrawShapes(hdc2, weight);
	if (isCreatePolygon && startPoint.x!=endPoint.x && startPoint.y!=endPoint.y)
	{
		MoveToEx(hdc2, startPoint.x, startPoint.y, NULL);
		LineTo(hdc2, endPoint.x, endPoint.y);
	}
	if (isPrintSelected)
	{
		HPEN hPen = CreatePen(HS_CROSS, 1, BLACK_PEN);
		SelectObject(hdc2, hPen);
		SelectObject(hdc2, GetStockObject(NULL_BRUSH));
		Rectangle(hdc2, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
		DeleteObject(hPen);
	}

	if (zoom != 1)
	{
		XFORM xForm;
		xForm.eM11 = xForm.eM22 = zoom;
		xForm.eM12 = xForm.eM21 = xForm.eDx = xForm.eDy = 0;
		SetGraphicsMode(hdc, GM_ADVANCED);
		SetWorldTransform(hdc, &xForm);
	}
	BitBlt(hdc, 0, newClientRectTop+1, windowWidth, windowHeight, hdc2, 0, 0, SRCCOPY);
	SelectObject(hdc2, hold);
	ReleaseDC(hWnd, hdc);
	DeleteObject(hbm);
	DeleteDC(hdc2);
	ValidateRect(hWnd, &windowRect);
}

void FreeResources()
{
	DeleteEnhMetaFile(hmf);
	ClearShapesVector();
	delete(filename);
}

void ExitApplication()
{
	FreeResources();
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HDC hdc;
	RECT rt;

	switch (message)
	{
		case WM_CREATE:InitResource(hWnd);			
			break;
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
				break;
			SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);
			UpdateWindow(hWndToolBar);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Разобрать выбор в меню:
			switch (wmId)
			{
				case IDM_FILENEW:
					if (ShowSaveMessageBox(hWnd, isFileChanged))
					{
						if (hmf != NULL)
							DeleteEnhMetaFile(hmf);
						ResetAll(hWnd, &currentShapeId, &isFileChanged);
						filename->ResetFileName();
						filename->SetWindowCaption(hWnd);
						ZeroMemory(name, sizeof(name));
					}
					break;
				case IDM_FILEOPEN:
					if (ShowSaveMessageBox(hWnd,isFileChanged))
					{
						HENHMETAFILE newHmf = hmf;
						if ((newHmf = OpenEnhMetaFile(name)) != NULL)
						{
							hmf = newHmf;
							filename->SetFileName(name);
							filename->SetWindowCaption(hWnd);
							ResetAll(hWnd, &currentShapeId, &isFileChanged);
						}
						else
							DeleteEnhMetaFile(newHmf);
					}
					break;
				case IDM_FILESAVE:
					GetClientRect(hWnd, &rt);
					rt.top += clientRectTop;
					if (isPrintSelected)
					{
						if (!ShowSaveFileDialog("Открыть файл для печати"))
							return 1;
						PrintMetaFile(hWnd, startPoint, endPoint, rt);
						InvalidateRect(hWnd, NULL, FALSE);
						break;
					}
					if (isFileChanged)
					{
						if (strlen(name) == 0)
							if (!ShowSaveFileDialog("Открыть файл для записи"))
								return 1;
						filename->SetFileName(name);
						filename->SetWindowCaption(hWnd);
						hmf=SaveEnhanceMetafile(rt,filename->CreateTempFileName(name));
						isFileChanged = false;
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
					isPrintSelected = false;
					isCreatePolygon = false;
					ResetPointValues(&startPoint, &endPoint);
					break;
				case IDM_PAINTCAN:brushColor = ChooseColorProc(chooseColor[0]);
					break;
				case IDM_CHOOSECOLOR:penColor = ChooseColorProc(chooseColor[1]);
					break;
				case IDM_WEIGHT:DialogBox(hInst, MAKEINTRESOURCE(IDD_WEIGHTCHANGEBOX), hWnd, WeightChangeDialog);
					break;
				case IDM_PRINT:
					isPrintSelected = true;
					isCreatePolygon = false;
					ResetPointValues(&startPoint, &endPoint);
					currentShapeId = 0;
					break;
				case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:SendMessage(hWnd, WM_CLOSE, 0, 0);
					break;
				default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_LBUTTONDOWN:
			if (isPrintSelected)
			{
				startPoint = endPoint = SetNewPoint(MAKEPOINTS(lParam));
				break;
			}
			if (currentShapeId != 0)
			{
				isFileChanged = true;
				if (!isCreatePolygon)
				{
					currentShape = CreateNewShape(shapeFactoryMap.find(currentShapeId)->second);
					currentShape->SetPoint(SetNewPoint(MAKEPOINTS(lParam)));
				}
				if (currentShapeId == IDM_POLYGON || currentShapeId==IDM_POLYGONLINE)
				{
					endPoint = SetNewPoint(MAKEPOINTS(lParam));
					if (startPoint.x == 0 && startPoint.y == 0)
						startPoint = endPoint;
					if (!isCreatePolygon)
						isCreatePolygon = true;
					InvalidateRect(hWnd, NULL, FALSE);
				}
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT currentMousePos;
				POINTS tmpPoint = SetNewPoint(MAKEPOINTS(lParam));
				currentMousePos.x = tmpPoint.x;
				currentMousePos.y = tmpPoint.y;
				MouseMove(hWnd, mousePos, currentMousePos);
				mousePos = currentMousePos;
				break;
			}
		case WM_LBUTTONUP:
			if (currentShapeId != 0 && isCreatePolygon)
			{
				startPoint =SetNewPoint(MAKEPOINTS(lParam));
				currentShape->SetPoint(startPoint);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			if (isPrintSelected)
			{
				SendMessage(hWnd, WM_COMMAND, IDM_FILESAVE, 0);
				isPrintSelected = false;
			}
			break;
		case WM_RBUTTONDOWN:					
			isCreatePolygon = false;
			ResetPointValues(&startPoint, &endPoint);
			break;
		case WM_MBUTTONDOWN:
			{
				POINTS tmpPoint = SetNewPoint(MAKEPOINTS(lParam));
				mousePos.x = tmpPoint.x;
				mousePos.y = tmpPoint.y;
				SetCursor(handCursor);
				isMoving = true;
			}
			break;
		case WM_MBUTTONUP:
			SetCursor(crossCursor);
			isMoving = false;
			break;
		case WM_MOUSEWHEEL:MouseWheel(hWnd, wParam);
			break;
		case WM_CHAR:
			if (currentShape!=NULL && currentShapeId == IDM_TEXT)
			{
				Text* temp = dynamic_cast<Text*>(currentShape);
				temp->SetChar(wParam);
				InvalidateRect(hWnd, NULL, FALSE);
			}
			break;
		case WM_PAINT:	Drawing(hWnd);
			break;
		case WM_CLOSE:
			if (ShowSaveMessageBox(hWnd,isFileChanged))
				DestroyWindow(hWnd);
			break;
		case WM_DESTROY:ExitApplication();
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
