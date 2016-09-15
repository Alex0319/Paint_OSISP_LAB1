#pragma once
class Shape
{
	public:
		Shape();
		Shape(COLORREF,int,COLORREF);
		~Shape();
		virtual void Draw(HDC,int) = 0;
		virtual void SetPoint(POINTS);
		void RecalculateCoordinates(int, int,int,int);
	protected:
		POINT* points;
		int pointsCount;
		HPEN hPen;
		HBRUSH hBrush;
};
