
#ifndef _BULLET_H
#define _BULLET_H
#include "cSprite.h"

class Bullet : public cSprite
{
private:
	
	int speed;

public:
	Bullet();
	void update(double deltaTime);		// Rocket update method
	

};
#endif
