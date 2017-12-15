#include "Bullet.h"

Bullet::Bullet()
{
	
}

void Bullet::update(double deltaTime)
{

	setSpritePos({ getSpritePos().x , getSpritePos().y - 5 });



}
