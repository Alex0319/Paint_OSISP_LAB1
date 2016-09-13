#pragma once

class EllipseShape :public Shape
{
	public:
		EllipseShape();
		EllipseShape(COLORREF,int,COLORREF);
		~EllipseShape();
		void Draw(HDC,int);
		void SetPoint(POINTS);
};
