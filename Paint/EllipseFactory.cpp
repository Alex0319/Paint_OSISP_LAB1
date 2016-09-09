#include "stdafx.h"

EllipseFactory::EllipseFactory()
{
}


EllipseFactory::~EllipseFactory()
{
}

Shape* EllipseFactory::CreateShape()
{
	return new EllipseShape();
}
