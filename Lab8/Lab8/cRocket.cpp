/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"
   
//global variable (find better way to this)




/*
=================================================================
Defualt Constructor
=================================================================
*/
cRocket::cRocket() : cSprite()
{
	this->rocketVelocity = { 0, 0 };
	health = 3;
	 
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(double deltaTime)
{
	if (this->getSpritePos().x < 0) this->setSpritePos({ 0 , this->getSpritePos().y });
	if (this->getSpritePos().x > WINDOW_WIDTH - this->getSpritePos().w) this->setSpritePos({ WINDOW_WIDTH - this->getSpritePos().w , this->getSpritePos().y });
	this->setSpritePos({ this->getSpritePos().x + this->getSpriteTranslation().x, this->getSpritePos().y  });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cRocket::setRocketVelocity(SDL_Point rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
SDL_Point cRocket::getRocketVelocity()
{
	return rocketVelocity;
}

//damages player
void cRocket::damage()
{
	health--;
}

//sends health
int cRocket::getHealth()
{
	return health;
}

//restores the player after the game
void cRocket::revive()
{
	health = 3;
}






/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	rocketVelocity += spriteVelocityAdd;
//
//	spritePos2D += rocketVelocity * deltaTime;
//
//	rocketVelocity *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
//		{
//			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and asteroid to false
//				(*asteroidIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
//	while (bulletIterartor != theRocketBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theRocketBullets.erase(bulletIterartor);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			(*bulletIterartor)->renderCollisionBox();
//			++bulletIterartor;
//		}
//	}
//}