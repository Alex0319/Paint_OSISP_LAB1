#pragma once

class ShapeFactory
{
public:
	ShapeFactory();
	~ShapeFactory();
	virtual Shape* CreateShape() = 0;
};

