/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;




cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

//Lap 9 V
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();

/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{
	
}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	//restarting everything to base values
	loop = true;
	score = 0;
	restart = true;
	healthOnce = true;

	timer = 0; //Players timer
	e_timer = 0; //Enemies timer
	srand(time(0)); //Seeding rand with my pcs time

	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "Bullet","theRocket","theBackground","e_bullet"};
	texturesToUse = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png", "Images\\rocketSprite.png", "Images\\starscape1024x768.png", "Images\\e_bullet.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Store the textures

	// Creating the Buttons
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 },{ 400, 300 },{ 400, 300 },{ 500, 500 },{ 400, 300 },{ 740, 500 },{ 400, 300 } };
	
	//Adding the textures
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;




	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}

	//the Title
	gameTextList = { "Space Outvaders" };
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	// Create text Textures
	gameTextNames = { "TitleTxt", "CreateTxt", "DragDropTxt", "GameOver", "Victory" , "instructions_menu", "help1", "help2" , "newsystem" };
	gameTextList = { "Space Outvaders", "PlaceHolder", "PlacerHolder", "GameOver", "Victory!", "Instructions","A + D to move.", "Spacebar to fire.", "Score + Health =" };
	
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 228, 213, 238, 255 }, { 0, 0, 0, 0 }));
	}


	// Load game sounds
	soundList = { "theme", "shot", "explosion", "e_laser" , "playerDamage" , "eDamage" ,"click"};
	soundTypes = { MUSIC, SFX, SFX, SFX, SFX , SFX ,SFX };
	soundsToUse = { "Audio/theme.wav", "Audio/laserTest.wav", "Audio/explosion2.wav", "Audio/e_laser-gun.wav" , "Audio/damage.wav", "Audio/paind.wav" , "Audio/click.wav" };
	
	//adding sounds
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	//looping the main theme
	theSoundMgr->getSnd("theme")->play(-1);
	
	//rendering the background and the rocket
	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	theRocket.setSpritePos({ 512 - theTextureMgr->getTexture("theRocket")->getTWidth()/2, 600 });
	theRocket.setTexture(theTextureMgr->getTexture("theRocket"));
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	theRocket.setRocketVelocity({ 0, 0 });
	

	// Create vector array of Ships
	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos({ (200 * astro), 200 });
		//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
		theAsteroids[astro]->setActive(true);
	}



}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		this->getInput();
		this->update(elapsedTime, theRenderer);
		this->render(theSDLWND, theRenderer);
		theAreaClicked = { -1, -1 };
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	
	//Telling it what to render in each gamestate
	SDL_RenderClear(theRenderer);	//First

	switch (theGameState)
	{
	case MENU:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 350, 80, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		/*tempTextTexture = theTextureMgr->getTexture("CreateTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		tempTextTexture = theTextureMgr->getTexture("DragDropTxt");
		pos = { 10, 40, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);*/
		
		// Render Button
		theButtonMgr->getBtn("instructions_btn")->render(theRenderer, &theButtonMgr->getBtn("instructions_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("instructions_btn")->getSpritePos(), theButtonMgr->getBtn("instructions_btn")->getSpriteScale());
		theButtonMgr->getBtn("instructions_btn")->setSpritePos({ 10, 375 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 10, 325 });
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 10, 425 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case INSTRUCTIONS:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10,10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		
		tempTextTexture = theTextureMgr->getTexture("instructions_menu");
		pos = { 330, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		tempTextTexture = theTextureMgr->getTexture("help1");
		pos = { 200, 340, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		tempTextTexture = theTextureMgr->getTexture("help2");
		pos = { 200, 380, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		
		
		// Render Button

		theButtonMgr->getBtn("play_btn")->setSpritePos({ 10, 325 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 5, 375 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
	}
	break;

	case PLAYING:
	{
		

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each asteroid in the vector array
		for (int draw = 0; draw < theAsteroids.size(); draw++)
		{
			theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
		}
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });

		//render the score
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });
		
		//render the health
		tempTextTexture = theTextureMgr->getTexture("Health");
		pos = { 10, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });

		// render the rocket
		theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());

		for (int i = 0; i < theBullets.size(); i++)
		{
			theBullets[i]->render(theRenderer, &theBullets[i]->getSpriteDimensions(), &theBullets[i]->getSpritePos(), theBullets[i]->getSpriteRotAngle(), &theBullets[i]->getSpriteCentre(), theBullets[i]->getSpriteScale());
		}

		//render the bullets
		for (int i = 0; i < the_e_bullets.size(); i++)
		{
			the_e_bullets[i]->render(theRenderer, &the_e_bullets[i]->getSpriteDimensions(), &the_e_bullets[i]->getSpritePos(), the_e_bullets[i]->getSpriteRotAngle(), &the_e_bullets[i]->getSpriteCentre(), the_e_bullets[i]->getSpriteScale());
		}

		

		

	}
	break;
	case END:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//render the score
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });
		
		//render the health
		tempTextTexture = theTextureMgr->getTexture("Health");
		pos = { 10, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });

		//Victory or death
		if (score > 4)
		{
			tempTextTexture = theTextureMgr->getTexture("Victory");
			pos = { 400, 80, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });
		}
		else
		{
			tempTextTexture = theTextureMgr->getTexture("GameOver");
			pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });
		}

		tempTextTexture = theTextureMgr->getTexture("Combo");
		pos = { 10, 150, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });


		tempTextTexture = theTextureMgr->getTexture("Highscore");
		pos = { 600, 80, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, { 1, 1 });


		//buttons
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 5, 375 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 10, 425 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());


		
		

	}
	break;

	
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	
	SDL_RenderPresent(theRenderer);	//End
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime, SDL_Renderer* theRenderer)
{
	//cout << theGameState << endl;




	// Check Button clicked and change state
	if (theGameState == MENU || theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);


	}


	bool skip = false;

	if (theGameState == INSTRUCTIONS && !skip)
	{
		skip = true;
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);

	}

	if (theGameState == MENU && !skip)
	{
		skip = true;
		theGameState = theButtonMgr->getBtn("instructions_btn")->update(theGameState, INSTRUCTIONS, theAreaClicked);

	}

	if (theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);

	}



	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);

	//converting score and health so they can be displayed 
	string sScore = to_string(score);
	sScore = "Score: " + sScore;
	LPCSTR lpsScore = sScore.c_str();

	string shealth = to_string(theRocket.getHealth());
	shealth = "Health: " + shealth;
	LPCSTR lpsHealth = shealth.c_str();


	string sCombo = to_string(combo);
	sCombo = "Final Score: " + sCombo;
	LPCSTR lpsCombo = sCombo.c_str();

	string sHighscore = to_string(highscore);
	sHighscore = "Highscore: " + sHighscore;
	LPCSTR lpsHighscore = sHighscore.c_str();



	gameTextList = { "Asteroids", lpsScore, lpsHealth, lpsCombo, lpsHighscore };

	if (theTextureMgr->getTexture("Score") != NULL) theTextureMgr->deleteTexture("Score");
	theTextureMgr->addTexture("Score", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	if (theTextureMgr->getTexture("Health") != NULL) theTextureMgr->deleteTexture("Health");
	theTextureMgr->addTexture("Health", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 100, 255, 0, 255 }, { 0, 0, 0, 0 }));

	if (theTextureMgr->getTexture("Combo") != NULL) theTextureMgr->deleteTexture("Combo");
	theTextureMgr->addTexture("Combo", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 100, 255, 0, 255 }, { 0, 0, 0, 0 }));

	if (theTextureMgr->getTexture("Highscore") != NULL) theTextureMgr->deleteTexture("Highscore");
	theTextureMgr->addTexture("Highscore", theFontMgr->getFont("digital")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 100, 255, 0, 255 }, { 0, 0, 0, 0 }));




	//telling it what todo each gamestate.
	switch (theGameState)
	{

	case MENU:
	{
		score = 0;

		//doesnt needed but to stop it being called mor than once
		if (healthOnce == true)
		{
			theRocket.revive();
			healthOnce = false;
		}


		//Restarting the Game 
		if (restart == false)
		{
			for (int astro = 0; astro < 5; astro++)
			{
				theAsteroids.push_back(new cAsteroid);
				theAsteroids[astro]->setSpritePos({ (200 * astro), 200 });
				//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
				int randAsteroid = rand() % 4;
				theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[randAsteroid]));
				theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randAsteroid])->getTWidth(), theTextureMgr->getTexture(textureName[randAsteroid])->getTHeight());
				theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
				theAsteroids[astro]->setActive(true);
				restart = true;
			}
		}


	}
	break;

	case INSTRUCTIONS:
	{

	}
	break;

	case PLAYING:
	{
		//making the ships change back if they hit the edges

		for (int im = 0; im < theAsteroids.size(); im++)
		{
			if (theAsteroids[im]->getSpritePos().x < 0)
			{
				for (int i2 = 0; i2 < theAsteroids.size(); i2++)
				{
					theAsteroids[i2]->changeDirection();
				}
				break;
			}

			if (theAsteroids[im]->getSpritePos().x > 900)
			{
				for (int i2 = 0; i2 < theAsteroids.size(); i2++)
				{
					theAsteroids[i2]->changeDirection();
				}
				break;
			}
		}




		//Timers Players fire rate + enemy fire rate
		timer++;
		e_timer++;


		//Ending the Game
		if (theRocket.getHealth() < 1)
		{
			theGameState = END;
			combo = score + theRocket.getHealth();
			cGame::compareHighscore();
		}

		if (score > 4)
		{
			theGameState = END;
			combo = score + theRocket.getHealth();
			cGame::compareHighscore();
		}



		// Update the visibility and position of each asteriod
		vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
		while (asteroidIterator != theAsteroids.end())
		{
			if ((*asteroidIterator)->isActive() == false)
			{
				asteroidIterator = theAsteroids.erase(asteroidIterator);
			}
			else
			{
				(*asteroidIterator)->update(deltaTime);
				++asteroidIterator;
			}
		}

		// Update the Rockets position
		theRocket.update(deltaTime);

		//makes the bullets move and checks for detection
		for (int ib = 0; ib < theBullets.size(); ib++)
		{
			theBullets[ib]->update(deltaTime);



			for (int ia = 0; ia < theAsteroids.size(); ia++)
			{

				float bulletX = theBullets[ib]->getSpritePos().x;
				float bulletY = theBullets[ib]->getSpritePos().y;
				float astroX = theAsteroids[ia]->getSpritePos().x + theAsteroids[ia]->getSpritePos().w / 2;
				float astroY = theAsteroids[ia]->getSpritePos().y + theAsteroids[ia]->getSpritePos().h / 2;
				float distance = sqrtf(powf(bulletX - astroX, 2) + powf(bulletY - astroY, 2));


				if (distance < 50) {
					theSoundMgr->getSnd("eDamage")->play(0);
					score += 1;
					theAsteroids.erase(theAsteroids.begin() + ia);

					theBullets.erase(theBullets.begin() + ib);
					return;
				}
			}
			//if distance between this bullet and all asteroids is < than 80
			//then destroy bullet and the hit asteroid
			//print hit




		}
		//Makes enemy ships shoot 
		if (e_timer > 150)
		{
			theSoundMgr->getSnd("e_laser")->play(0);
			for (int iships = 0; iships < theAsteroids.size(); iships++)
			{

				the_e_bullets.push_back(new e_Bullet);

				//cout << theAsteroids[iships]->getSpriteRotAngle() << endl;

				if (theAsteroids[iships]->getSpriteRotAngle() > 5)
				{
					changeState = -2;
				}

				if (theAsteroids[iships]->getSpriteRotAngle() < 4 && (theAsteroids[iships]->getSpriteRotAngle() > -2))
				{
					changeState = 0;
				}

				if (theAsteroids[iships]->getSpriteRotAngle() < -2)
				{
					changeState = +2;
				}



				the_e_bullets[the_e_bullets.size() - 1]->setSpritePos({ theAsteroids[iships]->getSpritePos().x + theTextureMgr->getTexture("e_bullet")->getTWidth() , theAsteroids[iships]->getSpritePos().y });
				the_e_bullets[the_e_bullets.size() - 1]->setTexture(theTextureMgr->getTexture("e_bullet"));
				the_e_bullets[the_e_bullets.size() - 1]->setSpriteDimensions(theTextureMgr->getTexture("e_bullet")->getTWidth(), theTextureMgr->getTexture("e_bullet")->getTHeight());

			}
			e_timer = 0;

		}


		//makes enemy bullets move and hit detection
		for (int ie = 0; ie < the_e_bullets.size(); ie++)
		{


			the_e_bullets[ie]->update(deltaTime, changeState);


			float e_bulletX = the_e_bullets[ie]->getSpritePos().x + the_e_bullets[ie]->getSpritePos().w / 2;
			float e_bulletY = the_e_bullets[ie]->getSpritePos().y + the_e_bullets[ie]->getSpritePos().h /2;
			float rocketX = theRocket.getSpritePos().x + theRocket.getSpritePos().w / 2;
			float rocketY = theRocket.getSpritePos().y + theRocket.getSpritePos().h / 2;
			float playerSqDistance = powf(e_bulletX - rocketX, 2) + powf(e_bulletY - rocketY, 2);

			if (playerSqDistance < 500)
			{
				theSoundMgr->getSnd("playerDamage")->play(0);
				the_e_bullets.erase(the_e_bullets.begin() + ie);
				theRocket.damage();

			}

		}


	}
	break;

	case END:
	{
		//getting rid of anything that might persist after the restart
		healthOnce = true;

		for (int i = 0; i < theBullets.size(); i++)
		{
			theBullets.erase(theBullets.begin() + i);
		}


		for (int i = 0; i < the_e_bullets.size(); i++)
		{
			the_e_bullets.erase(the_e_bullets.begin() + i);
		}



		for (int i = 0; i < theAsteroids.size(); i++)
		{
			theAsteroids.erase(theAsteroids.begin() + i);
		}

		restart = false;

	}
	break;


	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
}

void cGame::compareHighscore()
{
	loadfile();
	if (highscore < combo) {
		highscore = combo;
		savefile();
	}
}

void cGame::savefile()
{
	fstream myfile;
	myfile.open("Save.txt");
	myfile << highscore;
	myfile.close();
}


void cGame::loadfile()
{
	string sFile;
	fstream myfile;
	myfile.open("Save.txt");
	myfile >> sFile;
	highscore = stoi(sFile);
	myfile.close();
}



void cGame::getInput()
{
	//getting all the inputs
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			loop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };

			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				loop = false;
				break;
			case SDLK_a:
			{

				theRocket.setSpriteTranslation({ -5, 0 });


			}
			break;

			case SDLK_d:
			{
				theRocket.setSpriteTranslation({ 5, 0 });
			}
			break;

			case SDLK_SPACE:
			{
				//firing the players bullets
				if (timer > 15)
				{
					theSoundMgr->getSnd("shot")->play(0);
					theBullets.push_back(new Bullet);
					theBullets[theBullets.size() - 1]->setSpritePos({ theRocket.getSpritePos().x + theTextureMgr->getTexture("Bullet")->getTWidth() / 2 , theRocket.getSpritePos().y });
					theBullets[theBullets.size() - 1]->setTexture(theTextureMgr->getTexture("Bullet"));
					theBullets[theBullets.size() - 1]->setSpriteDimensions(theTextureMgr->getTexture("Bullet")->getTWidth(), theTextureMgr->getTexture("Bullet")->getTHeight());
					timer = 0;
				}

			}
			break;





			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
			{
				//moving the ship
				theRocket.setSpriteTranslation({ 0, 0 });

			}
			break;

			case SDLK_d:
			{
				theRocket.setSpriteTranslation({ 0, 0 });
			}
			break;


			default:
				break;
			}

		default:
			break;
		}

	}
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}
