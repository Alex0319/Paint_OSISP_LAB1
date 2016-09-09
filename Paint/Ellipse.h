#pragma once

class EllipseShape :public Shape
{
	public:
		EllipseShape();
		~EllipseShape();
		void Draw(HDC,HPEN);
		void SetPoint(POINTS);
};
