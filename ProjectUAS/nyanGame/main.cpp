#include <sl.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include "properties.h"

bool isCollision(Nyan, Enemy);

int main()
{
	slWindow(1000, 500, "NYAN CAT JUMP", false);
// load assets
	// image
	int imgNyan = slLoadTexture("assets/nyan.png");
	int imgEnemy = slLoadTexture("assets/troll.png");
	int imgNyawa = slLoadTexture("assets/heart.png");
	int imgNyawaR = slLoadTexture("assets/heartbreak.png");
	int imgBack = slLoadTexture("assets/naturebg.jpg");
	int imgStartBG = slLoadTexture("assets/StartBG.png");
	int imgEndBG = slLoadTexture("assets/GameOverBG.png");
	int imgUS = slLoadTexture("assets/us.png");
	// sound
	int sndJump = slLoadWAV("assets/jump1.wav");
	int sndFall		= slLoadWAV("assets/jatoh.wav");
	int sndStart = slLoadWAV("assets/enter.wav");
	int sndEnd = slLoadWAV("assets/end.wav");
	// font
	int fontM = slLoadFont("assets/Pixels.ttf");
	

//Property
	const double G = -0.2;
	std::random_device rand;
	std::vector<Enemy> troll;
	for (int i = 0; i < 3; ++i)
	{
		double x = 1000 + i * 250;
		double y = (rand() % 5 + 1) * 113;
		troll.push_back({ x, y, 25 });
	}

	int score = 0;
	int play = 0;
	bool about = false;

	Nyan nyan{ 180, 250, 0, 0 ,40 };

	Text tim{ 0,1000, 280, 240, 200, 160, 500,0,500,500,0,1000};

// Game Mechanism
	while (!slShouldClose())
	{
	// Render Background 
		slSetForeColor(1, 1, 1, 1);
		slSprite(imgStartBG, 500, 250, 1000, 500);

	// START MENU
		tim.startY -= 8;
		if (tim.startY <= 240)
			tim.startY = 240;
		tim.closeY += 6;
		if (tim.closeY >= 210)
			tim.closeY = 210;
		tim.creditX -= 1;
		if (tim.creditX <= 920)
			tim.creditX = 920;
		slSetForeColor(0, 0, 0, 1);
		slSetTextAlign(SL_ALIGN_CENTER);
		slSetFont(fontM, 48);
		slText(500, tim.startY, "press [enter] to continue");
		slText(500, tim.closeY, "press [esc] to exit");
		slSetFont(fontM, 24);
		slText(tim.creditX, 10, "press [C] to see credit");

	// ENTER = START GAME
		if (slGetKey(SL_KEY_ENTER))
		{
			play = play += 1;
			slSoundPlay(sndStart);
			Sleep(300);
		}
		else if (slGetKey(SL_KEY_ESCAPE)) {
			slClose();
		}

	// CREDIT
		if (slGetKey('C'))
		{
			about = true;
			
		}
		// close
		else if (slGetKey('X'))
		{
			about = false;
			//reset
			tim.posXL = 0;
			tim.posXR = 1000;
			tim.posY1 = 284;
			tim.posY2 = 244;
			tim.posY3 = 204;
			tim.posY4 = 164;
			tim.exitX = 500;
			tim.exitY = 0;
			tim.startY = 500;
			tim.closeY = 0;
			tim.creditX = 1000;
		}
		if (about == true) {
			slSetForeColor(1, 1, 1, 1);
			slSprite(imgStartBG, 500, 250, 1000, 500);

			tim.posXL += 10;
			tim.posXR -= 10;
			if (tim.posXL >= 500 && tim.posXR <= 500)
			{
				tim.posXL = 500;
				tim.posXR = 500;
			}
				
			slSetForeColor(0, 0, 0, 1);
			slSetFont(fontM, 64);
			slText(tim.posXL, tim.posY1, "ahmad fillah alwy");
			slText(tim.posXR, tim.posY2, "andreas andrianto");
			slText(tim.posXL, tim.posY3, "bagas ridho ilhami");
			slText(tim.posXR, tim.posY4, "febrio pasha arwanda");
			
			tim.exitY += 3;
			if (tim.exitY >= 80)
				tim.exitY = 80;

			slSetForeColor(0, 0, 0, 1);
			slSetFont(fontM, 48);
			slText(tim.exitX, tim.exitY, "press [X] to back ");
		}
		

	// IF Enter Game Start
		if (play == 1 || play % 2 == 1) 
		{
			about = false;

		// Membatasi karakter agar tidak offside ke atas
			nyan.velY += G;
			nyan.posY += nyan.velY;
			if (nyan.posY >= 500)
			{
				nyan.posY = 500;
			}
		
		// Background IN GAME
			slSetForeColor(1, 1, 1, 1);
			slSprite(imgBack, 500, 250, 1000, 500);

		//	Gameplay
			if (nyan.health > 0)
			{
			// JUMP MECHANIC
				if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT) || slGetKey(' '))
				{
					nyan.velY = 3;
					
				}
				if (nyan.velY == 3)
					slSoundPlay(sndJump);

			// Render Cat
				slSetForeColor(1, 1, 1, 1);
				slSprite(imgNyan, nyan.posX, nyan.posY, 100, 75);

			// Render Enemy
				slSetForeColor(1, 1, 1, 1);
				for (const Enemy& e : troll)
					slSprite(imgEnemy, e.posX, e.posY, 100, 80);
				for (Enemy& e : troll)
				{
					e.posX -= 5;		// bergerak ke kiri
					if (e.posX <= 0)	// jika di tepi kiri, reset posisi
					{
						e.posX = 1000;
						e.posY = (rand() % 5 + 1) * 113;
						score++;		// update score
					}
					if (isCollision(nyan, e))
					{
						slSoundPlay(sndFall);
						nyan.health = nyan.health - 1;
						nyan.posY = 500;
						nyan.velY = 0;
						Sleep(600);
						for (int i = 0; i < troll.size(); ++i)
						{
							double x = 800 + i * 200;
							double y = (rand() % 3 + 1) * 100;
							troll[i].posX = x;
							troll[i].posY = y;
						}
					}
				}
				
			// Score
				slSetForeColor(0, 0, 0, 1);
				slSetFont(fontM, 48);
				slSetTextAlign(SL_ALIGN_LEFT);
				slText(20, 480, "score:");
				slText(100, 480, std::to_string(score).c_str());

			// Pause
				slSetFont(fontM, 24);
				slSetTextAlign(SL_ALIGN_CENTER);
				slText(920, 10, "press [enter] to pause");

			// FALL
				if (nyan.health > 0)
				{
					if (nyan.posY <= 0)
					{
						nyan.posY = 500;
						nyan.velY = 0;
						nyan.health = nyan.health - 1;
						Sleep(600);
						for (int i = 0; i < troll.size(); ++i)
						{
							double x = 800 + i * 200;
							double y = (rand() % 3 + 1) * 100;
							troll[i].posX = x;
							troll[i].posY = y;
						}
						slSoundPlay(sndFall);
					}
				}

			//Heart
				slSetForeColor(1, 1, 1, 1);
				if (nyan.health == 3)
				{
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawa, 905, 480, 30, 30);
					slSprite(imgNyawa, 930, 480, 30, 30);
				}
				else if (nyan.health == 2)
				{
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawa, 905, 480, 30, 30);
					slSprite(imgNyawaR, 930, 480, 52, 52);
				}
				else if (nyan.health == 1) {
					slSprite(imgNyawa, 880, 480, 30, 30);
					slSprite(imgNyawaR, 905, 480, 52, 52);
					slSprite(imgNyawaR, 930, 480, 52, 52);
					
				}
				else {
					slSprite(imgNyawaR, 880, 480, 52, 52);
					slSprite(imgNyawaR, 905, 480, 52, 52);
					slSprite(imgNyawaR, 930, 480, 52, 52);
					slSoundPlay(sndEnd);
					Sleep(1000); 
				}
			}
		// GAME OVER
			else
			{
				slSetForeColor(1, 1, 1, 1);
				slSprite(imgEndBG, 500, 250, 1000, 500);
				slSetForeColor(0, 0, 0, 1);
				tim.endY -= 5;
				if (tim.endY <= 180)
					tim.endY = 180;
				slSetTextAlign(SL_ALIGN_CENTER);
				slSetFont(fontM, 48);
				slText(515, tim.endY, "press [enter] to continue");
			// reset
				if (slGetKey(SL_KEY_ENTER))
				{
					nyan.health = 3;
					tim.endY = 500;
				}
				tim.startY = 500;
				tim.closeY = 0;
				tim.creditX = 1000;
				nyan.velY = 0;
				nyan.posY = 250;
				for (int i = 0; i < troll.size(); ++i)
				{
					double x = 800 + i * 200;
					double y = (rand() % 3 + 1) * 100;
					troll[i].posX = x;
					troll[i].posY = y;
				}
				score = 0;
			}
		}
		slRender();
	}
	slClose();
	return 0;
}


bool isCollision(Nyan c, Enemy e)
{
	double dx = e.posX - c.posX;
	double dy = e.posY - c.posY;
	double hit = e.radius + c.radius;
	double distance = sqrt(pow(dx, 2) + pow(dy, 2));
	if (distance <= hit)
		return true;
	return false;
}