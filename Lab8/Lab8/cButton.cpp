/*
=================
- cButton.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cButton.h"
#include "asteroidsGame.h"


static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

/*
=================================================================
Defualt Constructor
=================================================================
*/
cButton::cButton() : cSprite()
{
	// Load game sounds
	/*soundList = { "theme", "shot", "explosion", "e_laser" , "playerDamage" , "eDamage" ,"click" };
	soundTypes = { MUSIC, SFX, SFX, SFX, SFX , SFX ,SFX };
	soundsToUse = { "Audio/theme.wav", "Audio/laserTest.wav", "Audio/explosion2.wav", "Audio/e_laser-gun.wav" , "Audio/damage.wav", "Audio/paind.wav" , "Audio/click.wav" };

	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}*/
}


/*
=================================================================
Update the sprite position
=================================================================
*/


gameState cButton::update(gameState theCurrentGameState, gameState newGameState, SDL_Point theAreaClicked)
{
	SDL_Point areaClicked = theAreaClicked;
	if (areaClicked.x > this->getSpritePos().x && areaClicked.x < (this->getSpritePos().x + this->getSpriteDimensions().w) && areaClicked.y > this->getSpritePos().y && areaClicked.y < (this->getSpritePos().y + this->getSpriteDimensions().h))
	{
		this->buttonClickedRC.x = (int)(areaClicked.x - this->getSpritePos().x) / this->getSpriteDimensions().w;
		this->buttonClickedRC.y = (int)(areaClicked.y - this->getSpritePos().y) / this->getSpriteDimensions().h;
		this->clicked = true;
		return newGameState;
		//theSoundMgr->getSnd("click")->play(0);
	}
	return theCurrentGameState;
}

bool cButton::getClicked()
{
	return this->clicked;
}

void cButton::setClicked(bool state)
{
	this->clicked = state;
}