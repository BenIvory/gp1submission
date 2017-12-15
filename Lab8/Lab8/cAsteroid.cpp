/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cAsteroid::cAsteroid() : cSprite()
{
	this->asteroidVelocity = { 0, 0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cAsteroid::changeDirection()
{
	direction = !direction;
	
}


void cAsteroid::update(double deltaTime)
{
	
	
	/*if (getSpritePos().x < 0) left_right = 2;
	if (getSpritePos().x > 1024 - 128) left_right = -2;
	setSpritePos({getSpritePos().x + left_right ,getSpritePos().y });
	*/
	//cout << direction << endl;
	if (direction)
	{
		setSpritePos({ getSpritePos().x + 1 ,getSpritePos().y });
	}
	else
	{
		setSpritePos({ getSpritePos().x - 1  ,getSpritePos().y });
	}
	
	
	if (this->getSpriteRotAngle() < 15 && (state == true))
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() + (5.0f * deltaTime));
		
	}
	else
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() - (5.0f * deltaTime));
		state = false;
	}

	if (this->getSpriteRotAngle() < -10)
	{
		state = true;
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(SDL_Point AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}


/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}



