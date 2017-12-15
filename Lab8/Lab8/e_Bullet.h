#ifndef _e_BULLET_H
#define _e_BULLET_H
#include "cSprite.h"

class e_Bullet : public cSprite
{
private:

	int speed;

public:
	e_Bullet();
	void update(double deltaTime , int change);		// Rocket update method


};
#endif 
