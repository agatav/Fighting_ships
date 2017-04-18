#include "flyingobject.h"

class Ship :
	public FlyingObject
{
public:
	static const int SIZEX = 20;
	static const int SIZEY = 100;
	Ship(int _x, int _y) : FlyingObject(_x, _y, SIZEX, SIZEY) { }

	~Ship() { }
};
