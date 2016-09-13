#pragma once

class RectangleShape :public Shape
{
	public:
		RectangleShape();
		RectangleShape(COLORREF,int,COLORREF);
		~RectangleShape();
		void Draw(HDC,int);
		void SetPoint(POINTS);
};
