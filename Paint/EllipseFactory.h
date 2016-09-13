#pragma once
#include "ShapeFactory.h"

class EllipseFactory :	public ShapeFactory
{
public:
	EllipseFactory();
	~EllipseFactory();
	Shape* CreateShape(COLORREF,int,COLORREF);
};
