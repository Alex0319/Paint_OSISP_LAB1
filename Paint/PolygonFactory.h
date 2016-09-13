#pragma once

class PolygonFactory :	public ShapeFactory
{
	public:
		PolygonFactory();
		~PolygonFactory();
		Shape* CreateShape(COLORREF,int,COLORREF);
};

