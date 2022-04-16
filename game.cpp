//============================================================================
// Name        : game.cpp
// Author      : M.Usama Azam
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Dodge 'Em...
//============================================================================

#ifndef DODGE_CPP_
#define DODGE_CPP_
#include "util.h"
#include "CImg.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;

int Right=0,Left=0,Up=0,Down=0; 	 // Declaring Global Variables to use in the car's turn-movements function.
float x = 700;		 //x-coordinate of opponent car1 position
float y = 32;  		//y-coordinate of opponent car1 position
float x100 = 700;	 //x-coordinate of opponent car2 position
float y100 = 132;  	//y-coordinate of opponent car2 position
float x1 = 500; 	//x-coordinate of player car position	
float y10 = 32;  	//y-coordinate of player car position
int Menu1=0;		// Counter For Menu's display, help display and game display.
int Menu2=0;		// Menu2 is used for the new Menu display with additional option of continue.
bool Array[100]={};	// bool Array used in giftboxes.
int score=0;		// Score of the game
float z10=0;    	// Using this to pause the game and to resume
int lifes=3;		// Lifes of player	
int m=0;	// Counter For Next Levels
int GameLevel=1;	// GameLevels Counter
int Counter;	// Counter uses in gift-boxes.
int a=0;	// Using a as important variable in level 3 so opponent car can moves with double speed.
int HighScore; // highscore of the game.
int n=1; // using this when cars collide so that arena get refilled.
int END1000=0;
// seed the random numbers generator by current time (see the documentation of srand for further help)...

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) 
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}
void DisplayMenu()	// Function For Menu.
{
	DrawString( 350, 932, "(1)START A NEW GAME", colors[BROWN]);			
	DrawString( 350, 832, "(2)SEE HIGH-SCORES", colors[BROWN]);
	DrawString( 350, 732, "(3)HELP", colors[BROWN]);
	DrawString( 350, 632, "(4)EXIST", colors[BROWN]);
	
	if(Menu2==1)	// If Menu2==1 display additional option to resume the game.
	{
		DrawString( 350, 532, "(5)CONTINUE", colors[BROWN]);
	}
	
	
}
void DisplayHighScore()
{
	ifstream file;
	file.open("HighScore.txt");
	file>>HighScore;
	file.close();
	DrawString( 350, 1132, "Welcome To HighScore Page:", colors[BROWN]);
	DrawString( 150, 932,"Uptill Now The High Score Is:", colors[BROWN]);
	DrawString( 350, 582, "HIGHSCORE = ", colors[BROWN]);
	DrawString( 520, 582, Num2Str(HighScore), colors[BROWN]); 
	DrawString( 550, 132," Press Backspace To return To Menu.", colors[BROWN]);
}
void DisplayHelp()	// Function For Help.
{
	DrawString( 350, 1132, "Welcome To Guidelines Page:", colors[BROWN]);
	DrawString( 150, 932,"(1) Player Controls The Car turning movement with arrow keys and is allowed only if the car is at the place of", colors[BROWN]);
	DrawString( 150, 882,"      gap_turn.", colors[BROWN]);
	DrawString( 150, 832,"(2) Player car moves clockwise while opponent car moves counter clockwise.", colors[BROWN]);
	DrawString( 150, 732,"(3) Opponent Car will take a turn if only it makes it closer to the user car.", colors[BROWN]);
	DrawString( 150, 632,"(4) Collect All the Rectangular-Gift Boxes to complete the level and earns Score.", colors[BROWN]);
	DrawString( 150, 532,"(5) Player Has Three lifes to earn maximum score.", colors[BROWN]);
	DrawString( 150, 432,"(6) If complete the level then additional 100 points will be given.", colors[BROWN]);
	DrawString( 150, 332,"(7) Press '1' to start the game, '2' to show high-scores, '3' to show help, '4' and esc key to exist", colors[BROWN]);
	DrawString( 150, 282,"      In the Game pressed 'p' to pause and show a new menu.", colors[BROWN]);	
	DrawString( 550, 132," Press Backspace To return To Menu.", colors[BROWN]);

}
void beep()
{
	if((x+25)>=x1 && (x-25)<=x1)	// Setting range between player car's coordinates and giftboxes. 
	{		
		if((y+25)>=y10 && (y-50)<=y10)
		{
			cout<<"\a";
		}
	}

}
void GiftBoxes()	// Functions For Rectangular-GiftBoxes.
{
	Counter=0;
	for(int i=0,y=1180;i<10;i++,y-=100) // Drawing Rectangular Gift-Boxes.
	{
		if(i==5)
		{
			y-=220;	// To jump from Up to Down line.
		}
		for(int j=0,x=65;j<10;j++,x+=100)
		{
			if(j==5)
			{
				x+=220; // To jump from Left to Right line. 
			}
				if(Array[Counter]==0)
				{
					if((x+25)>=x1 && (x-25)<=x1)	// Setting range between player car's coordinates and giftboxes. 
					{
						if((y+25)>=y10 && (y-50)<=y10)
						{
							Array[Counter]=1;	
							score += 1;
							m += 1;
						}
					}
				}
			if(Array[Counter]==0)	// If False show giftboxes else if true then player car has taken the giftboxes.
			{
				if((j<4||j>5)||(i<4||i>5)) // Declaring condition so can avoid the gift-boxes inside the Fifth-Rectangular Box of Arena.
				{
					DrawRectangle(x,y,20,10,colors[rand()%100+10]);
				}
			}
			if((GameLevel==1||GameLevel==2||GameLevel==3||(GameLevel==4&&m!=96))&&Array[Counter]==1&&(m==0||n==0)&&lifes!=0)	// Bringing all the giftboxes back to arena if next levels starts.
			{
				m=0; // m=0 so that the next level starts only when all gift boxes are taken.
				Array[Counter]=0;
			}
			Counter+=1;
		}
	}
	n=1;  // so that refilled only once after collision.
}
void OpponentCar2()	// Function For 2nd Opponent Car.
{
if(lifes!=0)
{
if(GameLevel==4&&m!=96)
{
	// Drawing opponent car2
	float width10 = 15; 
	float height10 = 15;
	float* color10 = colors[DARK_BLUE];  // color of opponent car2.
	float radius10 = 5.0;
	DrawRoundRect(x100,y100,width10,height10,color10,radius10); // bottom left tyre
	DrawRoundRect(x100+width10*3,y100,width10,height10,color10,radius10); // bottom right tyre
	DrawRoundRect(x100+width10*3,y100+height10*4,width10,height10,color10,radius10); // top right tyre
	DrawRoundRect(x100,y100+height10*4,width10,height10,color10,radius10); // top left tyre
	DrawRoundRect(x100, y100+height10*2, width10, height10, color10, radius10/2); // body left rect
	DrawRoundRect(x100+width10, y100+height10, width10*2, height10*3, color10, radius10/2); // body center rect
	DrawRoundRect(x100+width10*3, y100+height10*2, width10, height10, color10, radius10/2); // body right rect
}
}
}
void GameLevels()	// Function For GameLevels.
{
	if(GameLevel==1||GameLevel==2||GameLevel==4)	// For Level 2 and 4 setting the initial coordinates of car's.
	{
		x = 700; 	//x-coordinate of opponent car1 position
 		y = 32;  	//y-coordinate of opponent car1 position
		x100 = 500;	 //x-coordinate of opponent car2 position
 		y100 = 232;	  //y-coordinate of opponent car2 position
 		x1 = 500;	 //x-coordinate of player car position	
 		y10 = 32; 	 //y-coordinate of player car position
		GiftBoxes();	// Calling GiftBoxes Funtions To Redraw all the giftboxes again.	
	}
	if(GameLevel==3)
	{
		x = 700; 	//x-coordinate of opponent car1 position
 		y = 32+a; 	 //y-coordinate of opponent car1 position	//Using +a condition so that coordinates match with car at its double speed.
 		x1 = 500; 	//x-coordinate of player car position	
 		y10 = 32;  	//y-coordinate of player car position
		GiftBoxes();	// Calling GiftBoxes Funtions To Redraw all the giftboxes again.
	}
}
void END()
{
	if(END1000==0)
	{
		z10=10000;
		END1000=1;
	}
	if(z10==0)
	{
		END1000=0;
		Menu1=0;
	}
}
void GameOver()	// Function for gameover
{
	DrawString( 650, 650, "Game Over", colors[MISTY_ROSE]);
	END();
}
void GameWin()	// Function for gamewin.
{
	DrawString( 650, 650, "You Won", colors[MISTY_ROSE]);
	END();
}
/*
 * Main Canvas drawing function.
 * */
//Board *b;

void GameDisplay() /*Displaying Game*/
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.

	glClearColor(0/*Red Component*/, 0,	//148.0/255/*Green Component*/,
			0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors

	// calling some fuid keyboard(unsigned char key, int x, int ynctions from util.cpp file to help students
	if(Menu1==0)	// If Menu1 is zero then display Menu.
	{
		DisplayMenu();	// Calling Menu Funtion.
		
	}
	if(Menu1==3)
	{
		DisplayHighScore();
	}
	if(Menu1==2)	// If Menu1 is two then display Help.
	{
		DisplayHelp();	// Calling Help Function.
	}
	if(Menu1==1)	// If Menu1 is one then Display Game.
	{
		GiftBoxes();	// Calling GiftBoxes Funtions.
	
		if(m==96)	// If m is equals to 96 inother words if player car takes all the giftboxes.
		{
			if(GameLevel<4)
			{
				score += 100;		// Give Bonus 100 reward to player.
				m=0;			// initializing m back to zero.
				GameLevel+=1;		// Moving on to the next Level.
				if(GameLevel==3)	
				{	
					a=10;		// If move to level 3 then increment 10 in a.
				}
				GameLevels();		// Calling GameLevel Function to move to next Level.
			}
		}		
		//HighScore
		ifstream file;
		file.open("HighScore.txt");
		file>>HighScore;
		file.close();
		if(HighScore<score)
		{
			HighScore=score;
			ofstream file;
			file.open("HighScore.txt");
			file << HighScore;
			file.close();
		}
		DrawString( 850, 1250, "SCORE = ", colors[MISTY_ROSE]); // Score of the game.
		DrawString( 960, 1250, Num2Str(score), colors[MISTY_ROSE]); // Score of the game.
		DrawString( 150, 1250, "LIFES = ", colors[MISTY_ROSE]); // Lifes of the Player.
		DrawString( 250, 1250, Num2Str(lifes), colors[MISTY_ROSE]); // Lifes of the Player.
		DrawString( 500, 1250, "GAMELEVEL = ", colors[MISTY_ROSE]); // GameLevel.
		DrawString( 680, 1250, Num2Str(GameLevel), colors[MISTY_ROSE]); // GameLevel.

if(lifes!=0)
{
if(GameLevel!=4||m!=96)
{		
		// Drawing opponent car1
		float width = 15; 
		float height = 15;
		float* color = colors[DARK_BLUE];  // color of opponent car.
		float radius = 5.0;
		DrawRoundRect(x,y,width,height,color,radius); // bottom left tyre
		DrawRoundRect(x+width*3,y,width,height,color,radius); // bottom right tyre
		DrawRoundRect(x+width*3,y+height*4,width,height,color,radius); // top right tyre
		DrawRoundRect(x,y+height*4,width,height,color,radius); // top left tyre
		DrawRoundRect(x, y+height*2, width, height, color, radius/2); // body left rect
		DrawRoundRect(x+width, y+height, width*2, height*3, color, radius/2); // body center rect
		DrawRoundRect(x+width*3, y+height*2, width, height, color, radius/2); // body right rect
}
}
		
		if(GameLevel==4)
		{
			a=0;	// If move to Level 4 then initialize a back to its original value.
			OpponentCar2();	// Calling Opponent Car2 Function so can have two's opponents cars in Level 4.
		}
		if(lifes!=0)
		{
		if(GameLevel!=4||m!=96)
		{	
			// Drawing player car	
			float width1 = 15; 
			float height1 = 15;
			float* color1 = colors[BROWN]; // color of players car.
			float radius1 = 5.0;
			DrawRoundRect(x1,y10,width1,height1,color1,radius1); // bottom left tyre
			DrawRoundRect(x1+width1*3,y10,width1,height1,color1,radius1); // bottom right tyre
			DrawRoundRect(x1+width1*3,y10+height1*4,width1,height1,color1,radius1); // top right tyre
			DrawRoundRect(x1,y10+height1*4,width1,height1,color1,radius1); // top left tyre
			DrawRoundRect(x1, y10+height1*2, width1, height1, color1, radius1/2); // body left rect
			DrawRoundRect(x1+width1, y10+height1, width1*2, height1*3, color1, radius1/2); // body center rect
			DrawRoundRect(x1+width1*3, y10+height1*2, width1, height1, color1, radius1/2); // body right rect
		}
		}
	
		// Drawing Arena
		int gap_turn = 120;  // Gaps For cars to turn in roads.
		int sx = 40;  // x-coordinate of a line. 		
		int sy = 15;  // y-coordinate of a line.	
		int swidth = 1200/2 - gap_turn/2; // half width.
		int sheight = 10;  // Thickness of a line.
		if(GameLevel==1)
		{
			float *scolor = colors[AQUA]; // Color for a line.
		
		for(int i=0,x=0;i<5;i++)  // Using for loop to draw multiple rectangle to make a complete Arena.
		{
			DrawRectangle(sx, sy, swidth, sheight, scolor); // bottom left line.
			DrawRectangle(sx + swidth + gap_turn, sy, swidth, sheight, scolor); // bottom right line.
			DrawRectangle(sx+swidth*2+gap_turn, (sy-10)+sheight, sheight*2, swidth, scolor); // right down line.
			DrawRectangle(sx+swidth*2+gap_turn, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor); // right up line.
			DrawRectangle(sx + swidth + gap_turn, sy+1200-x, swidth, sheight, scolor); // top left line.
			DrawRectangle(sx, sy+1200-x, swidth, sheight, scolor); // top right line.
			DrawRectangle(sx-sheight*2, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor); // left up line.
			DrawRectangle(sx-sheight*2, (sy-10)+sheight, sheight*2, swidth, scolor); // left down line.
	
			sx += 100;  // implementing 100 in x-coordinate.
			sy += 100;  // implementing 100 in y-coordinate.
			x += 200;  // x is used here to decrease the y-coordinate of top left and right line to make proper Rectangle.
			swidth = (1200-x)/2 - gap_turn/2; // Decreasing the length of a line by using x.
	
			if(i==3)  // using condition for final rectangle of Arena.
			{
				gap_turn -= 120; // for the final rectangle keeping gap-turn zero '0'. 
				swidth = (1200-x)/2; // increasing the length to fill the gap_turn decrease.
			}
		}
		}
		if(GameLevel==2)
		{
			float *scolor2 = colors[TOMATO]; // Color for a line.
		
		for(int i=0,x=0;i<5;i++)  // Using for loop to draw multiple rectangle to make a complete Arena.
		{
			DrawRectangle(sx, sy, swidth, sheight, scolor2); // bottom left line.
			DrawRectangle(sx + swidth + gap_turn, sy, swidth, sheight, scolor2); // bottom right line.
			DrawRectangle(sx+swidth*2+gap_turn, (sy-10)+sheight, sheight*2, swidth, scolor2); // right down line.
			DrawRectangle(sx+swidth*2+gap_turn, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor2); // right up line.
			DrawRectangle(sx + swidth + gap_turn, sy+1200-x, swidth, sheight, scolor2); // top left line.
			DrawRectangle(sx, sy+1200-x, swidth, sheight, scolor2); // top right line.
			DrawRectangle(sx-sheight*2, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor2); // left up line.
			DrawRectangle(sx-sheight*2, (sy-10)+sheight, sheight*2, swidth, scolor2); // left down line.
	
			sx += 100;  // implementing 100 in x-coordinate.
			sy += 100;  // implementing 100 in y-coordinate.
			x += 200;  // x is used here to decrease the y-coordinate of top left and right line to make proper Rectangle.
			swidth = (1200-x)/2 - gap_turn/2; // Decreasing the length of a line by using x.
	
			if(i==3)  // using condition for final rectangle of Arena.
			{
				gap_turn -= 120; // for the final rectangle keeping gap-turn zero '0'. 
				swidth = (1200-x)/2; // increasing the length to fill the gap_turn decrease.
			}
		}
		}
		if(GameLevel==3)
		{
			float *scolor3 = colors[SILVER]; // Color for a line.
		
		for(int i=0,x=0;i<5;i++)  // Using for loop to draw multiple rectangle to make a complete Arena.
		{
			DrawRectangle(sx, sy, swidth, sheight, scolor3); // bottom left line.
			DrawRectangle(sx + swidth + gap_turn, sy, swidth, sheight, scolor3); // bottom right line.
			DrawRectangle(sx+swidth*2+gap_turn, (sy-10)+sheight, sheight*2, swidth, scolor3); // right down line.
			DrawRectangle(sx+swidth*2+gap_turn, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor3); // right up line.
			DrawRectangle(sx + swidth + gap_turn, sy+1200-x, swidth, sheight, scolor3); // top left line.
			DrawRectangle(sx, sy+1200-x, swidth, sheight, scolor3); // top right line.
			DrawRectangle(sx-sheight*2, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor3); // left up line.
			DrawRectangle(sx-sheight*2, (sy-10)+sheight, sheight*2, swidth, scolor3); // left down line.
	
			sx += 100;  // implementing 100 in x-coordinate.
			sy += 100;  // implementing 100 in y-coordinate.
			x += 200;  // x is used here to decrease the y-coordinate of top left and right line to make proper Rectangle.
			swidth = (1200-x)/2 - gap_turn/2; // Decreasing the length of a line by using x.
	
			if(i==3)  // using condition for final rectangle of Arena.
			{
				gap_turn -= 120; // for the final rectangle keeping gap-turn zero '0'. 
				swidth = (1200-x)/2; // increasing the length to fill the gap_turn decrease.
			}
		}
		}
		if(GameLevel==4)
		{
			float *scolor4 = colors[GOLD]; // Color for a line.
		
		for(int i=0,x=0;i<5;i++)  // Using for loop to draw multiple rectangle to make a complete Arena.
		{
			DrawRectangle(sx, sy, swidth, sheight, scolor4); // bottom left line.
			DrawRectangle(sx + swidth + gap_turn, sy, swidth, sheight, scolor4); // bottom right line.
			DrawRectangle(sx+swidth*2+gap_turn, (sy-10)+sheight, sheight*2, swidth, scolor4); // right down line.
			DrawRectangle(sx+swidth*2+gap_turn, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor4); // right up line.
			DrawRectangle(sx + swidth + gap_turn, sy+1200-x, swidth, sheight, scolor4); // top left line.
			DrawRectangle(sx, sy+1200-x, swidth, sheight, scolor4); // top right line.
			DrawRectangle(sx-sheight*2, sy+sheight+swidth+gap_turn, sheight*2, swidth, scolor4); // left up line.
			DrawRectangle(sx-sheight*2, (sy-10)+sheight, sheight*2, swidth, scolor4); // left down line.
	
			sx += 100;  // implementing 100 in x-coordinate.
			sy += 100;  // implementing 100 in y-coordinate.
			x += 200;  // x is used here to decrease the y-coordinate of top left and right line to make proper Rectangle.
			swidth = (1200-x)/2 - gap_turn/2; // Decreasing the length of a line by using x.
	
			if(i==3)  // using condition for final rectangle of Arena.
			{
				gap_turn -= 120; // for the final rectangle keeping gap-turn zero '0'. 
				swidth = (1200-x)/2; // increasing the length to fill the gap_turn decrease.
			}
		}
		}
		if(lifes==0)
		{
			GameOver();
		}
		if(GameLevel==4&&m>95)
		{
			GameWin();
		}
	}
	glutSwapBuffers();
	
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) // Function For Player's Car Turn-Movements.
{
		if((x1>=50&&x1<=1170)&&y10>=540&&y10<=640) // Player can only turn in gap_turn places.
		{
			if(x1!=50) // Player can not turn left when car is in the x-coordinate '50' to keep car in the Arena.
			{
				if(x1!=870) // Player can not turn left when car is in the x-coordinate '870' to keep car in the Arena.
				{
					if (key	== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/)
					{
						Left = 50;  // If pressed G_K_L then moves the player-car to left.
						x1 -= Left;	
					}
				}
			}
	
			if(x1!=1170) // Player can not turn right when car is in the x-coordinate '1170' to keep car in the Arena.
			{
				if(x1!=350) // Player can not turn right when car is in the x-coordinate '350' to keep car in the Arena.
				{ 
					if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) 
					{
						Right = 50;  // If pressed G_K_R then moves the player-car to right.
						x1 += Right;	
					}
				}
			}
		}
		if((y10>=32&&y10<=1132)&&x1>=550&&x1<=650) // Player can only turn in gap_turn places.
		{
			if(y10!=1132) // Player can not turn up when car is in the y-coordinate '1132' to keep car in the Arena.
			{
				if(y10!=332) // Player can not turn up when car is in the y-coordinate '332' to keep car in the Arena.
				{
					if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) 
					{
						Up = 50;  // If G_K_U pressed then moves the player-car to up.
						y10 += Up;	
					}
				}
			}

			if(y10!=32) // Player can not turn down when car is in the y-coordinate '32' to keep car in the Arena.
			{
				if(y10!=832) // Player can not turn down when car is in the y-coordinate '832' to keep car in the Arena.
				{
					if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) 
					{
						Down = 50;  // If G_K_D pressed then moves the player-car to down.
						y10 -= Down;	
					}
				}
			}
		}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) 
{
	if(key==27||key==52)/* Escape key ASCII*/	// IF also pressed 4 then exit the game.
	{
		exit(1); // exit the program when escape key is pressed.
	}

	if(key=='b'||key=='B')	// If pressed B then moves to next Level.
	{
		if(GameLevel<4)	// Maximum Level can Reached.
		{
			GameLevel+=1;
			m=0;
			if(GameLevel==3)	
			{	
				a=10;		// If move to level 3 then increment 10 in a.
			}
			GameLevels();
		}
	}

	if(key=='p'||key=='P')	// If P is pressed then pause the game and shows the menu.
	{
		Menu1=0;	// Showing the Menu.
		Menu2=1;	// Adding additional option in the Menu to Resume the game.
		z10=10000;	// Incrementing 10000 in z10 to pause.
	}
	if(key==49&&Menu1==0)	// If 1 is pressed the start the new game.
	{
		GameLevel=1;	// GameLevel Reset To 1.
		lifes=3;	//Lifes Back to 3.
		a=0;		// a to zero so opponent car be in its original coordinate roadways.
		z10=0;		// To start the game.
		m=0;		// So condition of giftboxes gets true.
		score=0;	// score reset to zero.		
		GameLevels();	// Calling Functions to reset the game.
		Menu1=1;	// Display the Game.
	}
	if(key==51&&Menu1==0)	// If 3 is pressed then Help Guidelines will be shown.		// Menu1==0 condition so can go into Help guidelines only when at Menu page.
	{
		Menu1=2;
	}
	if(key==8&&Menu1==2)	// If BackSpace is pressed then return to Menu from Help Guidelines.	// Menu1==2 condition so can use backspace only when in help guidelines page.
	{
		Menu1=0;
	}
	if(key==50&&Menu1==0)	// If 2 is pressed then Display HighScore  		// Menu1==0 condition so only show HighScore When at Menu page.
	{
		Menu1=3;
	}
	if(key==8&&Menu1==3)	// If BackSpace is pressed then return to Menu from HighScore Page.	// Menu1==3 condition so can use backspace only when in HighScore page.
	{
		Menu1=0;
	}
	if(key==53&&Menu2==1&&Menu1==0)	// If 5 is pressed in otherwords additional option of Menu1 to resume after the game is paused. Then resume the game.
	{	
		z10=0;		// Initializing z10 back to zero to resume the game.
		Menu2=0;	// So can only used resume button when game is paused.
		Menu1=1;	// Displaying Game.	
	}

	if(key==32/* Space key ASCII*/)  //to drive the player car at the double speed than normal.
	{
		if((y10==32&&(x1>=50&&x1<=1170))||(y10==132&&(x1>=150&&x1<=1070))||(y10==232&&(x1>=250&&x1<=970))||(y10==332&&(x1>=350&&x1<=870)))
		{
			x1-=10;
		}
		if((x1==50&&(y10>=32&&y10<=1132))||(x1==150&&(y10>=132&&y10<=1032))||(x1==250&&(y10>=232&&y10<=932))||(x1==350&&(y10>=332&&y10<=832)))
		{
			y10+=10;
		}
		if((y10==1132&&(x1>=50&&x1<=1170))||(y10==1032&&(x1>=150&&x1<=1070))||(y10==932&&(x1>=250&&x1<=970))||(y10==832&&(x1>=350&&x1<=870)))
		{
			x1+=10;
		}	
		if((x1==1170&&(y10>=32&&y10<=1132))||(x1==1070&&(y10>=132&&y10<=1032))||(x1==970&&(y10>=232&&y10<=932))||(x1==870&&(y10>=332&&y10<=832)))
		{
			y10-=10;
		}	
	}
	glutPostRedisplay();
}
/*
 * This function is called after every 60.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) 
{

	// Moving Opponent Car1 Counter-Clockwise
	// If GameLevel==3 then a is equals to 10 and setting the range into even no coordinates so can drive at double speed. 
	if((y==32+a&&(x>=50-a&&x<=1170-a))||(y==132+a&&(x>=150-a&&x<=1070-a))||(y==232+a&&(x>=250-a&&x<=970-a))||(y==332+a&&(x>=350-a&&x<=870-a)))
	{
		if(GameLevel!=3)	// In other Levels drive at normal speed.
		{
			x+=10;
		}
		if(GameLevel==3)	// In GameLevel 3 drive at double speed.
		{
			x+=20;
		}
	}
	if((x==1170-a&&(y>=32+a&&y<=1132+a))||(x==1070-a&&(y>=132+a&&y<=1032+a))||(x==970-a&&(y>=232+a&&y<=932+a))||(x==870-a&&(y>=332+a&&y<=832+a)))
	{
		if(GameLevel!=3)	// In other Levels drive at normal speed.
		{
			y+=10;
		}
		if(GameLevel==3)	// In GameLevel 3 drive at double speed.
		{
			y+=20;
		}
	}
	if((y==1132+a&&(x>=50-a&&x<=1170-a))||(y==1032+a&&(x>=150-a&&x<=1070-a))||(y==932+a&&(x>=250-a&&x<=970-a))||(y==832+a&&(x>=350-a&&x<=870-a)))
	{
		if(GameLevel!=3)	// In other Levels drive at normal speed.
		{
			x-=10;
		}
		if(GameLevel==3)	// In GameLevel 3 drive at double speed.
		{
			x-=20;
		}
	}	
	if((x==50-a&&(y>=32+a&&y<=1132+a))||(x==150-a&&(y>=132+a&&y<=1032+a))||(x==250-a&&(y>=232+a&&y<=932+a))||(x==350-a&&(y>=332+a&&y<=832+a)))
	{
		if(GameLevel!=3)	// In other Levels drive at normal speed.
		{
			y-=10;
		}
		if(GameLevel==3)	// In GameLevel 3 drive at double speed.
		{
			y-=20;
		}
	}
	// Moving Opponent Car2 Counter-Clockwise
	if(GameLevel==4)
	{
		if((y100==32&&(x100>=50&&x100<=1170))||(y100==132&&(x100>=150&&x100<=1070))||(y100==232&&(x100>=250&&x100<=970))||(y100==332&&(x100>=350&&x100<=870)))
		{
			x100+=10;
		}
		if((x100==1170&&(y100>=32&&y100<=1132))||(x100==1070&&(y100>=132&&y100<=1032))||(x100==970&&(y100>=232&&y100<=932))||(x100==870&&(y100>=332&&y100<=832)))
		{
			y100+=10;
		}
		if((y100==1132&&(x100>=50&&x100<=1170))||(y100==1032&&(x100>=150&&x100<=1070))||(y100==932&&(x100>=250&&x100<=970))||(y100==832&&(x100>=350&&x100<=870)))
		{
			x100-=10;
		}	
		if((x100==50&&(y100>=32&&y100<=1132))||(x100==150&&(y100>=132&&y100<=1032))||(x100==250&&(y100>=232&&y100<=932))||(x100==350&&(y100>=332&&y100<=832)))
		{
			y100-=10;
		}
	}
	// Moving Player Car Clockwise
if(lifes!=0)
{
	if((y10==32&&(x1>=50&&x1<=1170))||(y10==132&&(x1>=150&&x1<=1070))||(y10==232&&(x1>=250&&x1<=970))||(y10==332&&(x1>=350&&x1<=870))||((y10==82||y10==182||y10==282)&&x1>=570&&x1<=630))
	{
		x1-=10;
	}
	if((x1==50&&(y10>=32&&y10<=1132))||(x1==150&&(y10>=132&&y10<=1032))||(x1==250&&(y10>=232&&y10<=932))||(x1==350&&(y10>=332&&y10<=832))||((x1==100||x1==200||x1==300)&&y10>=550&&y10<=620))
	{
		y10+=10;
	}
	if((y10==1132&&(x1>=50&&x1<=1170))||(y10==1032&&(x1>=150&&x1<=1070))||(y10==932&&(x1>=250&&x1<=970))||(y10==832&&(x1>=350&&x1<=870))||((y10==1082||y10==982||y10==882)&&x1>=570&&x1<=630))
	{
		x1+=10;
	}	
	if((x1==1170&&(y10>=32&&y10<=1132))||(x1==1070&&(y10>=132&&y10<=1032))||(x1==970&&(y10>=232&&y10<=932))||(x1==870&&(y10>=332&&y10<=832))||((x1==1120||x1==1020||x1==920)&&y10>=550&&y10<=620))
	{
		y10-=10;
	}
}
	// Collision Dectector Between Cars.
	if((x+20)>=x1 && (x-20)<=x1)	// if the player car and opponent car1 strikes with eact other then restarting all cars movements to initial xy-coordinates.
	{				// ALso Loosing one life.
		if((y+20)>=y10 && (y-20)<=y10)
		{
			x100 = 700; //x-coordinate of opponent car2 position	
 			y100 = 132;  //y-coordinate of opponent car2 position
			x = 700; //x-coordinate of opponent car1 position
 			y = 32+a;  //y-coordinate of opponent car1 position
 			x1 = 500; //x-coordinate of player car position	
 			y10 = 32;  //y-coordinate of player car position
			n = 0;	// refilling arena with gift-boxes	
			lifes-=1; // loosing one life when collides
		}
	}
	if(GameLevel==4)
	{
		if((x100+20)>=x1 && (x100-20)<=x1)	// if the player car and opponent car2 strikes with eact other then restarting all cars movements to initial xy-coordinates.
		{					// Also Loosing one life.
			if((y100+20)>=y10 && (y100-20)<=y10)
			{
				x1 = 500; //x-coordinate of player car position
 				y10 = 32;  //y-coordinate of player car position
 				x100 = 700; //x-coordinate of opponent car2 position	
 				y100 = 132;  //y-coordinate of opponent car2 position
				x = 700; //x-coordinate of opponent car1 position
 				y = 32+a;  //y-coordinate of opponent car1 position
				n = 0;	// refilling arena with gift-boxes.			
				lifes-=1;	// loosing one life when collides.
			}
		}
	}
	// AI For Opponent Car1.
if(GameLevel!=1)	// Opponent Car1 can only take upward and downward turns in any roadway for the first level.
{
	if(((x>=1070-a&&x<=1170-a)||(x>=50-a&&x<=150-a))&&y==612+a) 
	{               
			if(x>x1) 
			{
					Left = 20; 
					x -= Left;	
				
			}
	}
	if(((x>=970-a&&x<=1070-a)||(x>=150-a&&x<=250-a))&&y==592+a) 
	{               
			if(x>x1) 
			{
					Left = 20; 
					x -= Left;	
				
			}
	}
	if(((x>=870-a&&x<=970-a)||(x>=250-a&&x<=350-a))&&y==572+a) 
	{               
			if((x>x1)&&x!=870-a) 
			{
					Left = 20; 
					x -= Left;	
				
			}
	}
	if(((x>=50-a&&x<=150-a)||(x>=1070-a&&x<=1170-a))&&y==572+a) 
	{               
			if(x<x1)
			{
					Right = 20; 
					x += Right;	
				
			}
	}
	if(((x>=150-a&&x<=250-a)||(x>=970-a&&x<=1070-a))&&y==592+a) 
	{               
			if(x<x1) 
			{
					Right = 20; 
					x += Right;	
				
			}
	}
	if(((x>=250-a&&x<=350-a)||(x>=870-a&&x<=970-a))&&y==612+a) 
	{               
			if((x<x1)&&x!=350-a) 
			{
					Right = 20; 
					x += Right;	
				
			}
	}
}

	if(((y>=1032+a&&y<=1132+a)||(y>=32+a&&y<=132+a))&&x==630-a) 
	{
		if(y-a<y10) 
		{
			Up = 20;  
			y += Up;	
		}
	}
	if(((y>=932+a&&y<=1032+a)||(y>=132+a&&y<=232+a))&&x==610-a) 
	{
		if(y-a<y10) 
		{
			Up = 20;  
			y += Up;	
		}
	}
	if(((y>=832+a&&y<=932+a)||(y>=232+a&&y<=332+a))&&x==590-a)
	{
		if((y-a<y10)&&y!=332+a) 
		{
			Up = 20;  
			y += Up;	
		}
	}
	if(((y>=1032+a&&y<=1132+a)||(y>=32+a&&y<=132+a))&&x==590-a) 
	{
		if(y-a>y10) 
		{
			Down = 20;  
			y -= Down;	
		}
	}
	if(((y>=932+a&&y<=1032+a)||(y>=132+a&&y<=232+a))&&x==610-a) 
	{
		if(y-a>y10) 
		{
			Down = 20; 
			y -= Down;	
		}
	}
	if(((y>=832+a&&y<=932+a)||(y>=232+a&&y<=332+a))&&x==630-a)
	{
		if((y-a>y10)&&y!=832+a) 
		{
			Down = 20;  
			y -= Down;	
		}
	}

if(GameLevel==4)	
{
	// AI for OpponentCar2
	if(((x100>=1070&&x100<=1170)||(x100>=50&&x100<=150))&&y100==612) 
	{               
			if(x100>x1) 
			{
					Left = 20; 
					x100 -= Left;	
				
			}
	}
	if(((x100>=970&&x100<=1070)||(x100>=150&&x100<=250))&&y100==592) 
	{               
			if(x100>x1) 
			{
					Left = 20; 
					x100 -= Left;	
				
			}
	}
	if(((x100>=870&&x100<=970)||(x100>=250&&x100<=350))&&y100==572) 
	{               
			if((x100>x1)&&x100!=870) 
			{
					Left = 20; 
					x100 -= Left;	
				
			}
	}
	if(((x100>=50&&x100<=150)||(x100>=1070&&x100<=1170))&&y100==572)
	{               
			if(x100<x1) 
			{
					Right = 20; 
					x100 += Right;	
				
			}
	}
	if(((x100>=150&&x100<=250)||(x100>=970&&x100<=1070))&&y100==592) 
	{               
			if(x100<x1) 
			{
					Right = 20; 
					x100 += Right;	
				
			}
	}
	if(((x100>=250&&x100<=350)||(x100>=870&&x100<=970))&&y100==612) 
	{               
			if((x100<x1)&&x100!=350) 
			{
					Right = 20; 
					x100 += Right;	
				
			}
	}

	if(((y100>=1032&&y100<=1132)||(y100>=32&&y100<=132))&&x100==630) 
	{
		if(y100<y10) 
		{
			Up = 20;  
			y100 += Up;	
		}
	}
	if(((y100>=932&&y100<=1032)||(y100>=132&&y100<=232))&&x100==610) 
	{
		if(y100<y10) 
		{
			Up = 20;  
			y100 += Up;	
		}
	}
	if(((y100>=832&&y100<=932)||(y100>=232&&y100<=332))&&x100==590) 
	{
		if((y100<y10)&&y100!=332) 
		{
			Up = 20;  
			y100 += Up;	
		}
	}

	if(((y100>=1032&&y100<=1132)||(y100>=32&&y100<=132))&&x100==590) 
	{
		if(y100>y10) 
		{
			Down = 20;  
			y100 -= Down;	
		}
	}
	if(((y100>=932&&y100<=1032)||(y100>=132&&y100<=232))&&x100==610) 
	{
		if(y100>y10) 
		{
			Down = 20;  
			y100 -= Down;	
		}
	}
	if(((y100>=832&&y100<=932)||(y100>=232&&y100<=332))&&x100==630) 
	{
		if((y100>y10)&&y100!=832) 
		{
			Down = 20;  
			y100 -= Down;	
		}
	}
}
	if(z10<=10000&&z10>0)
	{
		z10-=5000;
	}
	glutPostRedisplay();
	// once again we tell the library to call our Timer function after next 60/FPS
	
	glutTimerFunc(60.0+z10, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y)
{
	cout << x << " " << y << endl;
	glutPostRedisplay();
}
void MouseMoved(int x, int y) 
{

	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) 
{

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
			{
		cout << GLUT_DOWN << " " << GLUT_UP << endl;

	} else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
			{

	}
	glutPostRedisplay();
}
/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) 
{

	//b = new Board(60, 60); // create a new board object to use in the Display Function ...

	int width = 1280, height = 1280; // i have set my window size to be 1280 x 960
	//b->InitalizeBoard(width, height);
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("DODGE'EM(Khan's Version)"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.

	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...	

	glutTimerFunc(60.0+z10, Timer, 0);

	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse
	
	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif /* AsteroidS_CPP_ */
