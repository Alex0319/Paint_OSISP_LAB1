#pragma once

class RectangleShape :public Shape
{
public:
	RectangleShape();
	~RectangleShape();
	void Draw(HDC, HPEN);
	void SetPoint(POINTS);
};
