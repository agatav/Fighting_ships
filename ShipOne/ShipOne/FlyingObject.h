#ifndef FLYING_OBJECT_H
#define FLYING_OBJECT_H
#include <math.h>
class FlyingObject
{
private:
	int x, y;
	const int sizeX, sizeY;
public:
	int getX() const { return x; }
	void setX(int _x) { x = _x; }

	int getY() const { return y; }
	void setY(int _y) { y = _y; }


	bool inContact(const FlyingObject& otherObject) const
	{
		double currentDistance
			, minimumDistance;

		currentDistance = sqrt(pow((double)x - otherObject.x + sizeX - otherObject.sizeX, 2) +
			pow((double)y - otherObject.y + sizeY - otherObject.sizeY, 2));
		minimumDistance = sqrt(pow((double)(sizeX >> 1) + (otherObject.sizeX >> 1), 2) +
			pow((double)(sizeY >> 1) + (otherObject.sizeY >> 1), 2));
		if (currentDistance <= minimumDistance)
			return true;
		return false;
	}
	FlyingObject(int _x, int _y, int _sizeX, int _sizeY) : x(_x), y(_y), sizeX(_sizeX), sizeY(_sizeY) {}
	~FlyingObject() { }
};
#endif
