#include "e_Bullet.h"

e_Bullet::e_Bullet()
{

}

void e_Bullet::update(double deltaTime, int change)
{

	
	
	setSpritePos({ getSpritePos().x + change, getSpritePos().y + 5 });



	

}

