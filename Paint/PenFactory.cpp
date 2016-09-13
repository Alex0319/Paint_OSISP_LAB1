#include "stdafx.h"

PenFactory::PenFactory()
{
}


PenFactory::~PenFactory()
{
}

Shape* PenFactory::CreateShape(COLORREF color, int weight, COLORREF brushColor)
{
	return new Pen(color, weight,brushColor);
}