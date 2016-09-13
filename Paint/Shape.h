#pragma once
class Shape
{
public:
	Shape();
	Shape(COLORREF,int,COLORREF);
	~Shape();
	virtual void SetPoint(POINTS) = 0;
	virtual void Draw(HDC,int) = 0;
protected:
	POINTS startPoint, endPoint;
	HPEN hPen;
	HBRUSH hBrush;
};
