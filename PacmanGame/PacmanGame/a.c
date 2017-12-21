#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


#define I (15)
#define J (20)
#define UP (0)
#define DOWN (1)
#define LEFT (2)
#define RIGHT (3)
#define N (10)
#define MODE (100)

/*0: player*/
/*1: enemy*/
typedef struct
{
	int i;
	int j;
	int mode;
	/*0 normal*/
	/*1 fellit*/
}Player;

typedef struct
{
	int i;
	int j;
	int direction;
	int valid;
}Enemy;

/*0: blank*/
/*1: wall*/
/*2: coin*/
/*3: fellit*/
/*4: player*/
/*5: enemy + blank*/
/*6: enemy + coin*/
/*7: item*/

void printmap(int map[][J], Player player, Enemy* enemy, int enemy_cnt_init, int enemy_cnt, int coin_cnt, int life, int score)
{
	int i, j, k, IsEnemy;

	printf("LIFE : ");
	for (i = 0; i < life; i++)
		printf("♥");
	printf("\nSCORE : %d\n", score);
	printf("%d\n", player.mode);


	
	for (i = 0; i < I; i++)
	{
		for (j = 0; j < J; j++)
		{
			IsEnemy = 0;
			for (k = 0; k < enemy_cnt_init; k++)
			{
				if (!enemy[k].valid) continue;

				if (enemy[k].i == i&&enemy[k].j == j)
				{
					if (!player.mode)
						printf("♥");
					else
						printf("♡");
					IsEnemy = 1;
					break;
				}
			}

			if (IsEnemy) continue;

			if (i == player.i&&j == player.j)

				printf("●");

			else if (map[i][j] == 1)
				printf("■");
			else if (map[i][j] == 2)
				printf("⊙");
			else if (map[i][j] == 3)
				printf("※");
			else if (map[i][j] == 7)
				printf("♨");
			else if (map[i][j] == 0 || map[i][j] == 8 || map[i][j] == 9)
				printf("  ");
		}
		printf("\n");
	}
}

void EnemyMove(int map[][J], Player player, Enemy* enemy, int enemy_cnt_init)
{
	int i;

	for (i = 0; i < enemy_cnt_init; i++)
	{
		if (!enemy[i].valid) continue;

		if (enemy[i].direction == UP)
		{
			if (map[enemy[i].i - 1][enemy[i].j] == 1)
			{
				enemy[i].direction = LEFT;
			}
			else
			{
				if (map[enemy[i].i][enemy[i].j] == 5)
				{
					map[enemy[i].i][enemy[i].j] = 0;
				}
				else if (map[enemy[i].i][enemy[i].j] == 6)
				{
					map[enemy[i].i][enemy[i].j] = 2;
				}
				else if (map[enemy[i].i - 1][enemy[i].j] == 7)
				{
					map[enemy[i].i][enemy[i].j] = 3;
				}

				/**/

				enemy[i].i = enemy[i].i - 1;

				if (map[enemy[i].i - 1][enemy[i].j] == 0)
				{
					//enemy[i].i = enemy[i].i - 1;
					map[enemy[i].i][enemy[i].j] = 5;
				}
				else if (map[enemy[i].i - 1][enemy[i].j] == 2)
				{
					//enemy[i].i = enemy[i].i - 1;
					map[enemy[i].i][enemy[i].j] = 6;
				}
				else if (map[enemy[i].i - 1][enemy[i].j] == 3)
				{
					//enemy[i].i = enemy[i].i - 1;
					map[enemy[i].i][enemy[i].j] = 7;
				}
				else if (enemy[i].i - 1 == player.i && enemy[i].j == player.j)
				{
					//라이프 감소
					//map[enemy[i].i][enemy[i].j] = 0;
					//enemy[i].i = enemy[i].i - 1;
				}
				else
				{

				}

				continue;
			}
		}

		if (enemy[i].direction == LEFT)
		{
			if (map[enemy[i].i][enemy[i].j - 1] == 1)
			{
				enemy[i].direction = DOWN;
			}
			else
			{
				if (map[enemy[i].i][enemy[i].j] == 5)
				{
					map[enemy[i].i][enemy[i].j] = 0;
				}
				else if (map[enemy[i].i][enemy[i].j] == 6)
				{
					map[enemy[i].i][enemy[i].j] = 2;
				}
				else if (map[enemy[i].i][enemy[i].j] == 7)
				{
					map[enemy[i].i][enemy[i].j] = 3;
				}

				/**/
				enemy[i].j = enemy[i].j - 1;

				if (map[enemy[i].i][enemy[i].j] == 0)
				{
					map[enemy[i].i][enemy[i].j] = 5;
				}
				else if (map[enemy[i].i][enemy[i].j] == 2)
				{
					map[enemy[i].i][enemy[i].j] = 6;
				}
				else if (map[enemy[i].i][enemy[i].j] == 3)
				{
					//enemy[i].j = enemy[i].j - 1;
					map[enemy[i].i][enemy[i].j] = 7;
				}
				else if (enemy[i].i == player.i && enemy[i].j == player.j)
				{
					//라이프 감소
					//map[enemy[i].i][enemy[i].j] = 0;
					//enemy[i].j = enemy[i].j - 1;
				}
				continue;
			}

		}

		if (enemy[i].direction == DOWN)
		{
			if (map[enemy[i].i + 1][enemy[i].j] == 1)
			{
				enemy[i].direction = RIGHT;
			}
			else
			{
				if (map[enemy[i].i][enemy[i].j] == 5)
				{
					map[enemy[i].i][enemy[i].j] = 0;
				}
				else if (map[enemy[i].i][enemy[i].j] == 6)
				{
					map[enemy[i].i][enemy[i].j] = 2;
				}
				else if (map[enemy[i].i][enemy[i].j] == 7)
				{
					map[enemy[i].i][enemy[i].j] = 3;
				}

				/**/
				enemy[i].i = enemy[i].i + 1;
				if (map[enemy[i].i][enemy[i].j] == 0)
				{
					map[enemy[i].i][enemy[i].j] = 5;
				}
				else if (map[enemy[i].i][enemy[i].j] == 2)
				{
					map[enemy[i].i][enemy[i].j] = 6;
				}
				else if (map[enemy[i].i][enemy[i].j] == 3)
				{
					//enemy[i].i = enemy[i].i + 1;
					map[enemy[i].i][enemy[i].j] = 7;
				}
				else if (enemy[i].i == player.i && enemy[i].j == player.j)
				{
					//라이프 감소
					//map[enemy[i].i][enemy[i].j] = 0;
					//enemy[i].i = enemy[i].i + 1;
				}
				continue;

			}
		}

		if (enemy[i].direction == RIGHT)
		{
			if (map[enemy[i].i][enemy[i].j+1] == 1)
			{
				enemy[i].direction = UP;
			}
			else
			{
				if (map[enemy[i].i][enemy[i].j] == 5)
				{
					map[enemy[i].i][enemy[i].j] = 0;
				}
				else if (map[enemy[i].i][enemy[i].j] == 6)
				{
					map[enemy[i].i][enemy[i].j] = 2;
				}
				else if (map[enemy[i].i][enemy[i].j] == 7)
				{
					map[enemy[i].i][enemy[i].j] = 3;
				}

				/**/
				enemy[i].j = enemy[i].j + 1;
				if (map[enemy[i].i][enemy[i].j] == 0)
				{
					map[enemy[i].i][enemy[i].j] = 5;
				}
				else if (map[enemy[i].i][enemy[i].j] == 2)
				{
					map[enemy[i].i][enemy[i].j] = 6;
				}
				else if (map[enemy[i].i][enemy[i].j] == 3)
				{
					//enemy[i].j = enemy[i].j+1;
					map[enemy[i].i][enemy[i].j] = 7;
				}
				else if (enemy[i].i == player.i && enemy[i].j == player.j)
				{
					//라이프 감소
					//map[enemy[i].i][enemy[i].j] = 0;
					//enemy[i].j = enemy[i].j+ 1;
				}
				continue;
			}
		}
	}

}

int MoveCheck(int map[][J], Player player, int direction)
{
	if (direction == UP)
	{
		if (map[player.i - 1][player.j] != 1) return 1;
		else return 0;
	}
	else if (direction == DOWN)
	{
		if (map[player.i + 1][player.j] != 1) return 1;
		else return 0;
	}
	else if (direction == LEFT)
	{
		if (map[player.i][player.j - 1] != 1) return 1;
		if (map[player.i][player.j] == 8) return 2;

		else return 0;
	}
	else if (direction == RIGHT)
	{
		if (map[player.i][player.j + 1] != 1) return 1;
		if (map[player.i][player.j] == 9) return 2;

		else return 0;
	}
}

int CollisionCheck(int map[][J], Player* player, Enemy* enemy, int* coin, int enemy_cnt_init, int* enemy_cnt, int *life, int*score)
{
	int i;
	/*coin*/
	if (map[(*player).i][(*player).j] == 2)
	{
		map[(*player).i][(*player).j] = 0;
		(*coin)--;
		(*score)+=10;

		return 0;
	}
	/*fellet*/
	else if (map[(*player).i][(*player).j] == 3)
	{
		(*player).mode = MODE;
		map[(*player).i][(*player).j] = 0;
		(*score) += 50;

		return 0;
	}
	else if (map[(*player).i][(*player).j] == 7)
	{
		map[(*player).i][(*player).j] = 0;
		(*score) += 200;

		return 0;
	}
	else if (map[(*player).i][(*player).j] == 5|| map[(*player).i][(*player).j] == 6)
	{
		for (i = 0; i < enemy_cnt_init; i++)
		{
			if (!enemy[i].valid) continue;
			if ((*player).i == enemy[i].i && (*player).j == enemy[i].j)break;
		}

		/*normal*/
		if (!(*player).mode)
		{
			(*life)--;

			return 1;
		}
		/*fellit*/
		else
		{
			enemy[i].valid = 0;
			(*enemy_cnt)--;
			map[(*player).i][(*player).j] = 0;
			(*score) += 200;

			return 2;
		}
	}
}

int main()
{
	int map[I][J] = { 0 };
	int enemy_cnt_init = 0, enemy_cnt, coin_cnt = 0, life = 3, score = 0;
	Player player;
	Enemy enemy[N];

	FILE* p;
	p = fopen("1.txt", "r");

	for (int i = 0; i < I; i++)
	{
		for (int j = 0; j < J; j++)
		{
			fscanf(p, "%c", &map[i][j]);
			map[i][j] -= '0';

			if (map[i][j] == 2)
			{
				coin_cnt++;
			}
			else if (map[i][j] == 4)
			{
				player.j = j;
				player.i = i;
				player.mode = 0;
				map[i][j] = 0;
			}
			else if (map[i][j] == 5)
			{
				enemy[enemy_cnt_init].i = i;
				enemy[enemy_cnt_init].j = j;
				enemy[enemy_cnt_init].valid = 1;
				enemy[enemy_cnt_init].direction = UP;
				enemy_cnt_init++;
				map[i][j] = 5;
			}
		}
		fscanf(p, "%*c");
	}

	fclose(p);

	enemy_cnt = enemy_cnt_init;

	printmap(map, player, enemy, enemy_cnt_init, enemy_cnt, coin_cnt, life, score);


	while (1)
	{
		char a;

		
		a = getch();

		if (a == 27)break;
		{
			if (a == 'w') { if (MoveCheck(map, player, UP)) player.i--; }
			else if (a == 's') { if (MoveCheck(map, player, DOWN))player.i++; }
			else if (a == 'a') { a = MoveCheck(map, player, LEFT); if (a == 1)player.j--; else if (a == 2) player.j = J - 1; }
			else if (a == 'd') { a = MoveCheck(map, player, RIGHT); if (a == 1)player.j++; else if (a == 2) player.j = 0; }
		}
		CollisionCheck(map, &player, enemy, &coin_cnt, enemy_cnt_init, &enemy_cnt, &life, &score);

		EnemyMove(map, player, enemy, enemy_cnt_init);

		CollisionCheck(map, &player, enemy, &coin_cnt, enemy_cnt_init, &enemy_cnt, &life, &score);
		if (player.mode) (player.mode)--;

		system("cls");

		printmap(map, player, enemy, enemy_cnt_init, enemy_cnt, coin_cnt, life, score);

		Sleep(33);

		if (life == 0)
		{
			printf("STAGE FAILED\n");
			break;
		}

		if (coin_cnt == 0)
		{
			printf("STAGE CLEAR\n");
			break;
		}
	}


	return 0;
}