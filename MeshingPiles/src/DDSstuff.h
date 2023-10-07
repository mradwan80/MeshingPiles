#pragma once
struct PointCoords
{
	float x, y, z;

	PointCoords() {}
	PointCoords(float x, float y, float z) : x(x), y(y), z(z) {}
};

class PileStruct
{
public:
	float depth;
	int pixel;
	float start, end;
};

