#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <conio.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define SCREEN_HEIGHT_2 17

#pragma warning(suppress : 4996)
#pragma warning(disable : 4996)

using namespace std;

class Position
{
protected:

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD CursorPosition;

public:

	void gotoxy(int x, int y)
	{
		CursorPosition.X = x;
		CursorPosition.Y = y;
		SetConsoleCursorPosition(console, CursorPosition);
	}

	void setCursor(bool visible, DWORD size)
	{
		if (size == 0)
			size = 20;

		CONSOLE_CURSOR_INFO lpCursor;
		lpCursor.bVisible = visible;
		lpCursor.dwSize = size;
		SetConsoleCursorInfo(console, &lpCursor);
	}
};

class Map : protected Position
{
public:

	Map() {}

	void drawBorder()
	{
		for (size_t i = 0; i < SCREEN_HEIGHT; i++)
		{
			for (size_t j = 0; j < SCREEN_HEIGHT_2; j++)
			{
				gotoxy(0 + j, i);
				cout << char(178);
				gotoxy(WIN_WIDTH - j, i);
				cout <<char(178);
			}
		}

		for (size_t i = 0; i < SCREEN_HEIGHT; i++)
		{
			gotoxy(SCREEN_WIDTH, i);
			cout << char(178);
		}
	}
};

class Car : public Position
{
protected:

	int carPos = WIN_WIDTH / 2;

public:

	Car() {}

	char car[4][4] =
	{
		char(255), char(201), char(187), char(255),
		char(178), char(185), char(204), char(178),
		char(178), char(185), char(204), char(178),
		char(255), char(200), char(188), char(255)
	};

	void drawCar()
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				gotoxy(j + carPos, i + 22);
				cout << car[i][j];
			}
		}
	}

	void eraseCar()
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				gotoxy(j + carPos, i + 22);
				cout << " ";
			}
		}
	}

};

class Enemy : Position
{
protected:

	int enemyY[3];
	int enemyX[3];
	int enemyFlag[3];

	Car car;

public:

	Enemy() {}

	void genEnemy(int ind)
	{
		enemyX[ind] = SCREEN_HEIGHT_2 + rand() % 33;
	}

	void drawEnemy(int ind)
	{
		if (enemyFlag[ind] == true)
		{
			gotoxy(enemyX[ind], enemyY[ind]);     cout << char(176) << char(203) << char(203) << char(176);
			gotoxy(enemyX[ind], enemyY[ind] + 1); cout << char(177) << char(202) << char(202) << char(177);
			gotoxy(enemyX[ind], enemyY[ind] + 2); cout << char(177) << char(203) << char(203) << char(177);
			gotoxy(enemyX[ind], enemyY[ind] + 3); cout << char(176) << char(202) << char(202) << char(176);
		}
	}

	void eraseEnemy(int ind)
	{
		if (enemyFlag[ind] == true)
		{
			gotoxy(enemyX[ind], enemyY[ind]);     cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
			gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
		}
	}

	void resetEnemy(int ind)
	{
		eraseEnemy(ind);
		enemyY[ind] = 1;
		genEnemy(ind);
	}
};

class Game : protected Position, protected Car, protected Enemy, protected Map
{
protected:

	int score;

public:

	Game() {}

	void instruction()
	{
		system("cls");
		cout << "\t------------------------------------" << endl;
		cout << "\t|            INSTRUCTIONS          |" << endl;
		cout << "\t------------------------------------" << endl;
		cout << "\t|Avoid cars by moving left or right|" << endl;
		cout << "\t------------------------------------" << endl;
		cout << "\t|      Press 'A' to move left      |" << endl;
		cout << "\t------------------------------------" << endl;
		cout << "\t|      Press 'D' to move right     |" << endl;
		cout << "\t------------------------------------" << endl;
		cout << "\t|       Press 'ESC' to exit        |" << endl;
		cout << "\t------------------------------------" << endl;
		cout << "\t| Press any key to go back to menu |" << endl;
		cout << "\t------------------------------------" << endl;
		_getch();
	}

	void updateScore()
	{
		Position::gotoxy(WIN_WIDTH + 7, 5);
		cout << "SCORE : " << score << endl;
	}

	int collision()
	{
		if (enemyY[0] + 4 >= 23)
		{
			if (enemyX[0] + 4 - carPos >= 1 && enemyX[0] + 4 - carPos <= 8)
			{
				return 1;
			}
		}
		return 0;
	}

	void Game_Over()
	{
		system("cls");
		cout << endl;
		cout << "\t" << char(201) << char(205) << char(205) << char(205) << char(205) << char(205) << char(205) << char(205);
		cout << char(205) << char(205) << char(205) << char(205) << char(187) << endl;
		cout << "\t" << char(186) << " GAME OVER " << char(186) << endl;
		cout << "\t" << char(200) << char(205) << char(205) << char(205) << char(205) << char(205) << char(205) << char(205);
		cout << char(205) << char(205) << char(205) << char(205) << char(188) << endl << endl;
		cout << "\t Your score : " << score << endl << endl;;
		cout << "\tPress any key to go back to menu ";
		_getch();
	}

	void play()
	{
		carPos = -1 + WIN_WIDTH / 2;
		score = 0;
		enemyFlag[0] = 1;
		enemyFlag[1] = 0;
		enemyY[0] = enemyY[1] = 1;

		system("cls");
		drawBorder();
		updateScore();
		genEnemy(0);
		genEnemy(1);

		Position::gotoxy(WIN_WIDTH + 7, 2);
		cout << " CAR GAME ";
		Position::gotoxy(WIN_WIDTH + 6, 4);
		cout << "----------";
		Position::gotoxy(WIN_WIDTH + 6, 6);
		cout << "----------";
		Position::gotoxy(WIN_WIDTH + 6, 12);
		cout << " Control ";
		Position::gotoxy(WIN_WIDTH + 6, 13);
		cout << "----------";
		Position::gotoxy(WIN_WIDTH + 2, 14);
		cout << " A Key - Left";
		Position::gotoxy(WIN_WIDTH + 2, 15);
		cout << " D key - Right";

		Position::gotoxy(18, 5);
		cout << " Press any key to start ";
		_getch();
		Position::gotoxy(18, 5);
		cout << "                        ";

		while (1)
		{
			if (_kbhit())
			{
				char ch = _getch();
				if (ch == 'a' || ch == 'A')
				{
					if (carPos > 18)
						carPos -= 4;
				}

				if (ch == 'd' || ch == 'D')
				{
					if (carPos < 50)
						carPos += 4;
				}

				if (ch == 27)
				{
					break;
				}
			}

			drawCar();
			drawEnemy(0);
			drawEnemy(1);
			if (collision() == 1)
			{
				Game_Over();
				return;
			}

			Sleep(50);
			eraseCar();
			eraseEnemy(0);
			eraseEnemy(1);

			if (enemyY[0] == 10)
				if (enemyFlag[1] == 0)
					enemyFlag[1] = 1;

			if (enemyFlag[0] == 1)
				enemyY[0] += 1;

			if (enemyFlag[1] == 1)
				enemyY[1] += 1;

			if (enemyY[0] > SCREEN_HEIGHT - 4)
			{
				resetEnemy(0);
				score++;
				updateScore();
			}

			if (enemyY[1] > SCREEN_HEIGHT - 4)
			{
				resetEnemy(1);
				score++;
				updateScore();
			}
		}
	}
};

int main()
{
	Position pos;
	pos.setCursor(0, 0);

	Game game;

	srand((unsigned)time(NULL));

	do {
		system("cls");
		pos.gotoxy(10, 5);
		cout << "-------------------";
		pos.gotoxy(10, 6);
		cout << "|     CAR GAME    |";
		pos.gotoxy(10, 7);
		cout << "-------------------";
		pos.gotoxy(10, 9);
		cout << "1. Start Game ";
		pos.gotoxy(10, 10);
		cout << "2. Instruction ";
		pos.gotoxy(10, 11);
		cout << "3. Quit ";
		pos.gotoxy(10, 13);
		cout << " Select option ";
		char choice = _getch();

		if (choice == '1') game.play();
		else if (choice == '2') game.instruction();
		else if (choice == '3') exit(0);

	} while (1);

	return 0;
}