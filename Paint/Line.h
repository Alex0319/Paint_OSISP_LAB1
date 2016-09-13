#pragma once

class Line :public Shape
{
public:
	Line();
	Line(COLORREF,int,COLORREF);
	~Line();
	void Draw(HDC,int);
	void SetPoint(POINTS);
};

