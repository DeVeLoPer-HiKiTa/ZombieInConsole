// my_console_game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include "windows.h"


using namespace std;

bool setCursorPosition(unsigned int x, unsigned int y);
bool draw_box();
bool redraw_player(const int key);
bool redraw_zombie(string command = "none");
//function to draw bullets
bool shoot(string command = "none");

//constants
const unsigned int WIDTH             = 75;
const unsigned int HEIGHT            = 18;
const unsigned int BARRIER_X         = 10;// x coordinate of barrier
const unsigned int NUMBER_OF_BULETTS = 100;//number of maximum bullets to draw

unsigned int    score                = 0;
bool            gameover             = false;
unsigned int    free_position_number = 0;//free coordinate for new bullet

//the coordinates of the objects
unsigned int player_x  = 0;
unsigned int player_y  = 0;
unsigned int first_zombie_x;
unsigned int first_zombie_y;
unsigned int second_zombie_x;
unsigned int second_zombie_y;
unsigned int bullets_positions[NUMBER_OF_BULETTS][2];

void setup()
{
	player_x = BARRIER_X - 2;
	player_y = HEIGHT / 2;
	first_zombie_x  = 33 + (rand() % (WIDTH / 2));
	first_zombie_y  = 1 + (rand() % (HEIGHT - 2));
	second_zombie_x = 33 + (rand() % (WIDTH / 2));
	second_zombie_y = 1 + (rand() % (HEIGHT - 2));

	//fill the array with coordinates which can not be to understand that the cell is free
	for (unsigned int i = 0; i < NUMBER_OF_BULETTS; ++i)
	{
		bullets_positions[i][0] = WIDTH + 1;
	}
}

int main()
{
	clock_t time_for_zombie  = clock();
	clock_t time_for_bullet  = clock();
	bool    shot             = false;

	setup();
	draw_box();
	redraw_player(' ');

	setCursorPosition(0, HEIGHT + 2);
	cout << "score: " << score;

	while (!gameover)
	{
		//if 200 milliseconds have passed
		if (clock() - time_for_zombie > 200)
		{
			time_for_zombie = clock();
			redraw_zombie("run");
		}

		if (clock() - time_for_bullet > 25)
		{
			time_for_bullet = clock();
			shoot("fly");
		}
		//if any key is pressed
		if (_kbhit())
		{
			//get the key
			int res = _getch();
			//if a w s d
			if (res == 119 || res == 97 || res == 100 || res == 115)
				redraw_player(res);
			//if q
			else if (res == 113)
				gameover = true;
			//if e
			else if (res == 101)
				shoot("shot");		
		}
	}
	//at the end of the game
	setCursorPosition(0, HEIGHT + 2);
	cout << endl;
	cout << "Game Over!!!" << endl;
	system("pause");
	return 0;
}

bool setCursorPosition(unsigned int x, unsigned int y)
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(output, pos);

	return true;
}

bool shoot(string command)
{
	if (command == "shot")
	{
		//searching a free position
		for (unsigned int i = 0; i <= NUMBER_OF_BULETTS; ++i)
		{
			if (bullets_positions[i][0] == WIDTH + 1)
			{
				free_position_number = i;
				break;
			}
		}

		bullets_positions[free_position_number][0] = player_x + 2;
		bullets_positions[free_position_number][1] = player_y;
		setCursorPosition(bullets_positions[free_position_number][0], bullets_positions[free_position_number][1]);
		cout << "*";
	}
	else if (command == "fly")
	{
		//we pass on each bullet
		for (unsigned int i = 0; i < NUMBER_OF_BULETTS; ++i)
		{
			//if need to draw
			if (bullets_positions[i][0] != WIDTH + 1)
			{
				//erase bullet
				setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
				cout << " ";

				//if the bullet didn't come out of the box
				if (bullets_positions[i][0] < WIDTH - 1)
				{
					//change position
					bullets_positions[i][0]++;

					//draw bullet again
					setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
					cout << "*";

					//if the bullet passed through the barrier
					if (bullets_positions[i][0] == BARRIER_X + 1)
					{
						setCursorPosition(BARRIER_X, bullets_positions[i][1]);
						cout << "#";
					}
					//if the bullet is out of the box
					else if (bullets_positions[i][0] == WIDTH - 1)
					{
						setCursorPosition(WIDTH - 1, bullets_positions[i][1]);
						cout << "#";
						bullets_positions[i][0] = WIDTH + 1;
					}
					//the following describes the cases of collision of bullets and zombies
					else if (bullets_positions[i][0] == first_zombie_x && bullets_positions[i][1] == first_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
						cout << " ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill first zombie");
					}
					else if (bullets_positions[i][0] - 1 == first_zombie_x && bullets_positions[i][1] == first_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0] - 1, bullets_positions[i][1]);
						cout << "  ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill first zombie");
					}
					else if (bullets_positions[i][0] + 1 == first_zombie_x && bullets_positions[i][1] == first_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
						cout << "  ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill first zombie");
					}
					else if (bullets_positions[i][0] == second_zombie_x && bullets_positions[i][1] == second_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
						cout << " ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill second zombie");
					}
					else if (bullets_positions[i][0] + 1 == second_zombie_x && bullets_positions[i][1] == second_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0], bullets_positions[i][1]);
						cout << "  ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill second zombie");
					}
					else if (bullets_positions[i][0] - 1 == second_zombie_x && bullets_positions[i][1] == second_zombie_y)
					{
						setCursorPosition(bullets_positions[i][0] - 1, bullets_positions[i][1]);
						cout << "  ";
						bullets_positions[i][0] = WIDTH + 1;
						redraw_zombie("kill second zombie");
					}
				}
			}
		}
	}
	else
		return false;

	return true;
}

bool redraw_zombie(string command)
{
	if (command == "run")
	{
		//erase zombies
		setCursorPosition(first_zombie_x, first_zombie_y);
		cout << " ";
		//change position
		first_zombie_x--;

		if (first_zombie_x == BARRIER_X)
		{
			gameover = true;
			return true;
		}
		//draw again
		setCursorPosition(first_zombie_x, first_zombie_y);
		cout << "Z";

		setCursorPosition(second_zombie_x, second_zombie_y);
		cout << " ";
		second_zombie_x--;

		if (second_zombie_x == BARRIER_X)
		{
			gameover = true;
			return true;
		}

		setCursorPosition(second_zombie_x, second_zombie_y);
		cout << "Z";
	}
	else if (command == "kill first zombie")
	{
		first_zombie_x = 33 + (rand() % (WIDTH / 2));
		first_zombie_y = 1 + (rand() % (HEIGHT - 2));

		score += 10;

		setCursorPosition(first_zombie_x, first_zombie_y);
		cout << "Z";
		
		setCursorPosition(0, HEIGHT + 2);
		cout << "score: " << score;
	}
	else if (command == "kill second zombie")
	{
		second_zombie_x = 33 + (rand() % (WIDTH / 2));
		second_zombie_y = 1 + (rand() % (HEIGHT - 2));

		score += 10;

		setCursorPosition(second_zombie_x, second_zombie_y);
		cout << "Z";

		setCursorPosition(0, HEIGHT + 2);
		cout << "score: " << score;
	}
	else
		return false;

	return true;
}

bool redraw_player(const int key)
{
	if (key == 97)
	{
		if (player_x > 1)
		{
			setCursorPosition(player_x, player_y);
			cout << "  ";
			player_x--;
			setCursorPosition(player_x, player_y);
			cout << "P-";
		}
	}
	else if (key == 100)
	{
		if (player_x < BARRIER_X - 2)
		{
			setCursorPosition(player_x, player_y);
			cout << "  ";
			player_x++;
			setCursorPosition(player_x, player_y);
			cout << "P-";
		}
	}
	else if (key == 119)
	{
		if (player_y > 1)
		{
			setCursorPosition(player_x, player_y);
			cout << "  ";
			player_y--;
			setCursorPosition(player_x, player_y);
			cout << "P-";
		}
	}
	else if (key == 115)
	{
		if (player_y < HEIGHT)
		{
			setCursorPosition(player_x, player_y);
			cout << "  ";
			player_y++;
			setCursorPosition(player_x, player_y);
			cout << "P-";
		}
	}
	else
	{
		setCursorPosition(player_x, player_y);
		cout << "P-";
	}

	return true;
}

bool draw_box()
{
	// top side
	for (unsigned int i = 0; i < WIDTH; i++)
		cout << "#";

	cout << endl;

	//we pass around the field
	for (unsigned int i = 0; i < HEIGHT; i++)
	{
		for (unsigned int j = 0; j < WIDTH; j++)
		{
			if (j == 0 || j == WIDTH - 1 || j == BARRIER_X)
				cout << "#";
			else
				cout << " ";
		}
		cout << endl;
	}
	//bottom side
	for (unsigned int i = 0; i < WIDTH; i++)
		cout << "#";

	return true;
}