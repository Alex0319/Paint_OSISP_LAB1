#pragma once
class Text :public Shape
{
	public:
		Text();
		Text(COLORREF, int,COLORREF);
		~Text();
		void Draw(HDC, int);
		void SetPoint(POINTS);
		void SetChar(TCHAR);
	private:
		COLORREF textColor,brushColor;
		LOGFONT font;
		HFONT hFont;
		std::string str;
		int count=0;
};
