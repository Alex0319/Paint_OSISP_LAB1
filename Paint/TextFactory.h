#pragma once

class TextFactory :	public ShapeFactory
{
	public:
		TextFactory();
		~TextFactory();
		Shape* CreateShape(COLORREF, int, COLORREF);
};
