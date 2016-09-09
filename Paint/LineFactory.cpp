#include "stdafx.h"

LineFactory::LineFactory()
{
}


LineFactory::~LineFactory()
{
}

Shape* LineFactory::CreateShape()
{
	return new Line();
}