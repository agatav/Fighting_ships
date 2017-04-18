#include "flyingobject.h"
enum Direction {
	LEFT, RIGHT
};

class Bullet :
	public FlyingObject
{
private:

	Direction dir;
public:
	Bullet& operator=(const Bullet& right)
	{
		if (this == &right) {
			return *this;
		}
		this->setX(right.getX());
		this->setY(right.getY());
		this->setDir(right.getDir());
		return *this;
	}
	Direction getDir() const { return dir; }
	void setDir(Direction _dir) { dir = _dir; }
	const static int SIZE = 20;
	void update();
	Bullet(int _x, int _y, Direction _dir) : FlyingObject(_x, _y, SIZE, SIZE), dir(_dir) { }
	Bullet() : FlyingObject(0, 0, SIZE, SIZE), dir(Direction::LEFT) { }
	~Bullet() { }
};

