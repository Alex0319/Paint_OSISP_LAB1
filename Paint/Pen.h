#pragma once

class Pen :	public PolygonLine
{
	public:
		Pen();
		Pen(COLORREF, int, COLORREF);
		~Pen();
		void Draw(HDC,int);
		void SetPoint(POINTS);
};

