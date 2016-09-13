#pragma once

class ShapeFactory
{
public:
	ShapeFactory();
	~ShapeFactory();
	virtual Shape* CreateShape(COLORREF,int,COLORREF) = 0;
};

