#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


#define N (10)

/*0: player*/
/*1: enemy*/
typedef struct
{
	int i;
	int j;
	int Type;
}Object;

/*0: blank*/
/*1: wall*/
/*2: item*/
/*3: fellit*/

void printmap(int map[][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (map[i][j] == 0)
				printf("  ");
			else if (map[i][j] == 1)
				printf("¡á");
			else if (map[i][j] == 2)
				printf("*");
			else if (map[i][j] == 3)
				printf("¡Ø");
			else if (map[i][j] == 4)
				printf("¡Ü");
			else if (map[i][j] == 5)
				printf("¢¾");
		}
		printf("\n");
	}
}

void move(int map[][N], int *x, int *y)
{

}

int main()
{
	int map[N][N] = { 0 };
	int ItemMap[N][N] = { 0 };
	Object Player;
	Player.Type = 0;
	Object *Enemy;

	FILE* p;
	p = fopen("1.txt", "r");

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			fscanf(p, "%c", &map[i][j]);
			map[i][j] -= '0';
			if (map[i][j] == 4)
			{
				Player.j = j;
				Player.i = i;
			}
		}
		fscanf(p, "%*c");
	}

	while (1)
	{
		printmap(map);

		char a;

		a = getch();

		if (a == 27)break;
		else if (a == 72)printf("UP");
		else if (a == 80)printf("DOWN");
		else if (a == 75)printf("LEFT");
		else if (a == 77)printf("RIGHT");

		Sleep(100);
		system("cls");
	}


	return 0;
}
