#pragma once

class PenFactory :	public ShapeFactory
{
	public:
		PenFactory();
		~PenFactory();
		Shape* CreateShape(COLORREF,int,COLORREF);
};

