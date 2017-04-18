#include "Arena.h"

Arena::Arena(const Direction _dir) : dir(_dir), playerBulletVec(list<Bullet>()), enemiesBulletVec(list<Bullet>()), bulletToSendVec(vector<int>(5))
, scorePoints(0), player(Ship(dir == Direction::LEFT ? 0 : WIDTH, -(HEIGHT >> 1))) { }



void Arena::createBullet()
{
	playerBulletVec.push_back(Bullet(player.getX() - (Ship::SIZEX), player.getY() + (Ship::SIZEY >> 1), dir == Direction::LEFT ? Direction::RIGHT : Direction::LEFT));
}

void Arena::createBullet(const int& y)
{
	if (dir == Direction::LEFT)
		enemiesBulletVec.push_back(Bullet(WIDTH - 20, y, Direction::LEFT));
	else
		enemiesBulletVec.push_back(Bullet(20, y, Direction::RIGHT));
}


void Arena::movePlayer(const int& delta)
{
	int newY = player.getY() + delta;
	if (newY < -Ship::SIZEY && newY > -(HEIGHT))
		player.setY(newY);
}
void Arena::update()
{
	bulletToSendVec.clear();
	for (auto i = enemiesBulletVec.begin(); i != enemiesBulletVec.end(); ++i)
	{
		if (i->inContact(player))
		{
			++scorePoints;
			enemiesBulletVec.erase(i);
			break;
		}
	}


	// determine bullets to send
	for (auto i = playerBulletVec.begin(); i != playerBulletVec.end();)
		if ((dir == Direction::LEFT && i->getX() > WIDTH)
			|| (dir == Direction::RIGHT && i->getX() < 0))
		{
			bulletToSendVec.push_back(i->getY());
			i = playerBulletVec.erase(i);
		}
		else ++i;

		for (auto i = enemiesBulletVec.begin(); i != enemiesBulletVec.end();)
			if ((dir == Direction::LEFT && i->getX() <= 0)
				|| (dir == Direction::RIGHT && i->getX() > WIDTH))
			{
				i = enemiesBulletVec.erase(i);
			}
			else ++i;


			//delete collided Bullets
			bool isDeleted;
			if (playerBulletVec.size() > 0 && enemiesBulletVec.size() > 0)
				for (auto i = playerBulletVec.begin(); i != playerBulletVec.end();)
				{
					isDeleted = false;
					for (auto j = enemiesBulletVec.begin(); j != enemiesBulletVec.end(); ++j)
						if (i->inContact(*j))
						{
							i = playerBulletVec.erase(i);
							enemiesBulletVec.erase(j);
							isDeleted = true;
							break;
						}
					if (!isDeleted) ++i;
				}

			if (dir == Direction::LEFT)
			{
				for (auto i = playerBulletVec.begin(); i != playerBulletVec.end(); ++i)
					i->setX(i->getX() + delta);
				for (auto i = enemiesBulletVec.begin(); i != enemiesBulletVec.end(); ++i)
					i->setX(i->getX() - delta);
			}
			else
			{
				for (auto i = playerBulletVec.begin(); i != playerBulletVec.end(); ++i)
					i->setX(i->getX() - delta);
				for (auto i = enemiesBulletVec.begin(); i != enemiesBulletVec.end(); ++i)
					i->setX(i->getX() + delta);
			}
}