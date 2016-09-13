#pragma once

class PolygonLine :	public PolygonShape
{
	public:
		PolygonLine();
		PolygonLine(COLORREF,int,COLORREF);
		~PolygonLine();
		void Draw(HDC,int);
		void SetPoint(POINTS);
};
