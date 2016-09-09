#include "stdafx.h"

RectangleFactory::RectangleFactory()
{
}


RectangleFactory::~RectangleFactory()
{
}

Shape* RectangleFactory::CreateShape()
{
	return new RectangleShape();
}
