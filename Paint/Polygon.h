#pragma once

class Polygon :	public Shape
{
	public:
		Polygon();
		~Polygon();
		void Draw(HDC, HPEN);
		void SetPoint(POINTS);
};
