#include <vector>
#include <list>
#include <algorithm>
#include "Bullet.h"
#include "Ship.h"

using namespace std;

class Arena
{
private:
	const Direction dir;
	static const int delta = 10;
	Ship player;
	list<Bullet> playerBulletVec;
	list<Bullet> enemiesBulletVec;
	vector<int> bulletToSendVec;
	int scorePoints;
public:
	int getScore() { return scorePoints; }
	static const int WIDTH = 1000;
	static const int HEIGHT = 1000;
	// containers for drawing, pass to window 
	const list<Bullet>& playerBulletsToDraw() const { return playerBulletVec; }
	const list<Bullet>& enemiesBulletsToDraw() const { return enemiesBulletVec; }
	const Ship& shipToDraw() const { return player; }

	void movePlayer(const int& deltaMove);
	const vector<int>& getBulletToSendVector() { return bulletToSendVec; }
	void createBullet();
	void createBullet(const int&);
	void update();
	Arena(const Direction);
	~Arena() {}
};


