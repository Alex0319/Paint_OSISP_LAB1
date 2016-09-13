#include "stdafx.h"
#include "Text.h"


Text::Text()
{
}

Text::Text(COLORREF color, int weight,COLORREF brushColor)
{
	textColor = color;
	this->brushColor = brushColor;
	font.lfHeight = 16;
	font.lfWidth = 0;
	font.lfWeight = weight;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = RUSSIAN_CHARSET | DEFAULT_CHARSET;
	font.lfOutPrecision = 0;// Устанавливает точность вывода
	font.lfClipPrecision = 0;// Устанавливает точность отсечения
	font.lfQuality = 0;// Устанавливает качество вывода
	font.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
	strcpy_s(font.lfFaceName, "VERDANA");
	hFont = CreateFontIndirect(&font);
}

Text::~Text()
{
	DeleteObject(hFont);
}

void Text::Draw(HDC hdc, int width)
{
	if (startPoint.x == 0 && startPoint.y == 0)
		startPoint.x =startPoint.y= 20;
	SelectObject(hdc, hFont);
	SetTextColor(hdc,textColor);
	SetBkColor(hdc, brushColor);
	TabbedTextOut(hdc, startPoint.x, startPoint.y,str.data(), str.size(), 0, 0, 0);
}

void Text:: SetPoint(POINTS point)
{
	startPoint = point;
}

void Text::SetChar(TCHAR textChar)
{
	str += textChar;
}
