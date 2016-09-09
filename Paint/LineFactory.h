#pragma once

class LineFactory :public ShapeFactory
{
public:
	LineFactory();
	~LineFactory();
	Shape* CreateShape();
};

