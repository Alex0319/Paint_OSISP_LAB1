#pragma once

class Line :public Shape
{
public:
	Line();
	~Line();
	void Draw(HDC,HPEN);
	void SetPoint(POINTS);
};

