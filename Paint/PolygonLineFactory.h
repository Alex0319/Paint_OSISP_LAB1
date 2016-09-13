#pragma once

class PolygonLineFactory :	public ShapeFactory
{
	public:
		PolygonLineFactory();
		~PolygonLineFactory();
		Shape* CreateShape(COLORREF,int,COLORREF);
};
