#pragma once
class Shape
{
public:
	Shape();
	~Shape();
	virtual void SetPoint(POINTS) = 0;
	virtual void Draw(HDC, HPEN) = 0;
protected:
	POINTS startPoint, endPoint;
	HPEN hPen;
};
