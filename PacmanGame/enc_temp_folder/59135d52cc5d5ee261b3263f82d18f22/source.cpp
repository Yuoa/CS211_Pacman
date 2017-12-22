#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


#define I (32)
#define J (42)
#define UP (1)
#define DOWN (2)
#define LEFT (3)
#define RIGHT (4)
#define N (4)
#define MODE (300000)
#define PLAYER_I (28)
#define PLAYER_J (21)
#define GHOST_I (11)
#define GHOST_J (21)

typedef struct
{
	int i, j;
	int i_offset, j_offset;
	int dir_now, dir_next;
	int mode;
	int speed;
}Player;

typedef struct
{
	int valid;
	int i, j;
	int i_offset, j_offset;
	int i_dest, j_dest;
	int dir_now, dir_next;
	int speed;
}Ghost;

void Init(const int datamap[][J], int map[][J], Player* player, Ghost* ghost, int level, int* coin_cnt_left)
{
	int i, j;

	for (i = 0; i < I; i++)
	{
		for (j = 0; j < J; j++)
		{
			map[i][j] = datamap[i][j];

			if (map[i][j] == 1)(*coin_cnt_left)++;
		}
	}

	(*player).i = PLAYER_I; (*player).j = PLAYER_J;
	(*player).i_offset = 0; (*player).j_offset = 0;
	(*player).dir_now = 0; (*player).dir_next = 0;
	(*player).mode = 0; (*player).speed = 4;

	for (i = 0; i < N; i++)
	{
		ghost[i].i = GHOST_I; ghost[i].j = GHOST_J;
		ghost[i].i_offset = 0; ghost[i].j_offset = 0;
		ghost[i].valid = 0; ghost[i].dir_now = UP; ghost[i].dir_next = UP;
	}

	if (level == 1)
	{
		ghost[0].speed = 10;
		ghost[0].valid = 1;
	}
	else if (level == 2)
	{
		ghost[0].speed = 10;
		ghost[1].speed = 8;
		ghost[0].valid = 1;
		ghost[1].valid = 1;
	}
	else if (level == 3)
	{
		ghost[0].speed = 10;
		ghost[1].speed = 8;
		ghost[2].speed = 6;
		ghost[0].valid = 1;
		ghost[1].valid = 1;
		ghost[2].valid = 1;
	}
	else if (level == 4)
	{
		ghost[0].speed = 10;
		ghost[1].speed = 8;
		ghost[2].speed = 6;
		ghost[3].speed = 4;
		ghost[0].valid = 1;
		ghost[1].valid = 1;
		ghost[2].valid = 1;
		ghost[3].valid = 1;
	}
}

void Input(Player* player)
{
	char a;

	if (kbhit())
	{
		a = getch();

		if (a == 'w') { if (player->dir_now == 0) { player->dir_now = UP; player->dir_next = UP; } else player->dir_next = UP; }
		else if (a == 's') { if (player->dir_now == 0) { player->dir_now = DOWN; player->dir_next = DOWN; } else player->dir_next = DOWN; }
		else if (a == 'a') { if (player->dir_now == 0) { player->dir_now = LEFT; player->dir_next = LEFT; } else player->dir_next = LEFT; }
		else if (a == 'd') { if (player->dir_now == 0) { player->dir_now = RIGHT; player->dir_next = RIGHT; } else player->dir_next = RIGHT; }
	}
}

void offsetupdate(int map[][J], Player* player, Ghost* ghost)
{
	int i;
	/*direction update*/
	if (player->dir_next != player->dir_now)
	{
		if (player->dir_next == UP)
		{
			if (player->j_offset == 0 && map[player->i - 1][player->j] < 3)
			{
				player->dir_now = UP;
			}
		}
		else if (player->dir_next == DOWN)
		{
			if (player->j_offset == 0 && map[player->i + 1][player->j] < 3)
			{
				player->dir_now = DOWN;
			}
		}
		else if (player->dir_next == LEFT)
		{
			if (player->i_offset == 0 && map[player->i][player->j - 1] < 3)
			{
				player->dir_now = LEFT;
			}
		}
		else
		{
			if (player->i_offset == 0 && map[player->i][player->j + 1] < 3)
			{
				player->dir_now = RIGHT;
			}
		}
	}
	
	/*move*/
	
	if (player->dir_now == UP|| player->dir_now == DOWN)
	{
		if (player->dir_now == UP)
		{
			if (map[player->i - 1][player->j] < 3)
				player->i_offset--;
		}
		else
		{
			if (map[player->i + 1][player->j] < 3)
				player->i_offset++;
		}
		if (player->i_offset == (-1)*(player->speed) / 2)
		{
			player->i--;
			player->i_offset = 0;
		}
		else if (player->i_offset == player->speed / 2 - 1)
		{
			player->i++;
			player->i_offset = 0;
		}
	}
	else
	{
		if (player->dir_now == LEFT)
		{
			if (map[player->i][player->j] == -10)
			{
				player->j = J - 1;
				player->j_offset = 0;
			}
			else if (map[player->i][player->j - 1] < 3)
				player->j_offset--;
		}
		else
		{
			if (map[player->i][player->j] == -11)
			{
				player->j = 0;
				player->j_offset = 0;
			}
			if (map[player->i][player->j + 1] < 3)
				player->j_offset++;
		}

		if (player->j_offset == (-1)*(player->speed) / 2)
		{
			player->j--;
			player->j_offset = 0;
		}
		else if (player->j_offset == player->speed / 2 - 1)
		{
			player->j++;
			player->j_offset = 0;
		}
	}

	/*ghost*/
	for (i = 0; i < N; i++)
	{
		if (ghost[i].valid == 0) continue;

		ghost[i].i_dest = player->i;
		ghost[i].j_dest = player->j;
		
		if (ghost[i].i_dest < ghost[i].i)
		{
			ghost[i].dir_now = UP;
			if (ghost[i].j_dest < ghost[i].j)
				ghost[i].dir_next = LEFT;
			else if (ghost[i].j_dest > ghost[i].j)
				ghost[i].dir_next = RIGHT;
		}
		else if (ghost[i].i_dest > ghost[i].i)
		{
			ghost[i].dir_now = DOWN;
			if (ghost[i].j_dest < ghost[i].j)
				ghost[i].dir_next = LEFT;
			else if (ghost[i].j_dest > ghost[i].j)
				ghost[i].dir_next = RIGHT;
		}
		else
		{
			if (ghost[i].j_dest < ghost[i].j)
				ghost[i].dir_now = ghost[i].dir_next = LEFT;
			else
				ghost[i].dir_now = ghost[i].dir_next = RIGHT;
		}

		///////////////////////////////


		if (ghost[i].dir_now == UP && ghost[i].dir_next == RIGHT && map[ghost[i].i - 1][ghost[i].j] > 2)
		{
			ghost[i].dir_now = ghost[i].dir_next = RIGHT;
		}
		else if (ghost[i].dir_now == DOWN && ghost[i].dir_next == RIGHT && map[ghost[i].i + 1][ghost[i].j] > 2)
		{
			ghost[i].dir_now = ghost[i].dir_next = RIGHT;
		}
		else if (ghost[i].dir_now == DOWN && ghost[i].dir_next == LEFT && map[ghost[i].i + 1][ghost[i].j] > 2)
		{
			ghost[i].dir_now = ghost[i].dir_next = LEFT;
		}
		else if (ghost[i].dir_now == UP && ghost[i].dir_next == LEFT && map[ghost[i].i - 1][ghost[i].j] > 2)
		{
			ghost[i].dir_now = ghost[i].dir_next = LEFT;
		}

		///////////////////////////

		if (ghost[i].valid == 1)
		{
			if (ghost[i].dir_now == UP || ghost[i].dir_now == DOWN)
			{
				if (ghost[i].dir_now == UP)
				{
					if (map[ghost[i].i - 1][ghost[i].j] < 3)
						ghost[i].i_offset--;
				}
				else
				{
					if (map[ghost[i].i + 1][ghost[i].j] < 3)
						ghost[i].i_offset++;
				}

				if (ghost[i].i_offset == (-1)*(ghost[i].speed) / 2)
				{
					ghost[i].i--;
					ghost[i].i_offset = 0;
				}
				else if (ghost[i].i_offset == ghost[i].speed / 2 - 1)
				{
					ghost[i].i++;
					ghost[i].i_offset = 0;
				}
			}
			else
			{
				if (ghost[i].dir_now == LEFT)
				{
					if (map[ghost[i].i][ghost[i].j - 1] < 3)
						ghost[i].j_offset--;
				}
				else
				{
					if (map[ghost[i].i][ghost[i].j + 1] < 3)
						ghost[i].j_offset++;
				}
				if (ghost[i].j_offset == (-1)*(ghost[i].speed) / 2)
				{
					ghost[i].j--;
					ghost[i].j_offset = 0;
				}
				else if (ghost[i].j_offset == ghost[i].speed / 2 - 1)
				{
					ghost[i].j++;
					ghost[i].j_offset = 0;
				}
			}
		}

	}
}

void colcheck(int map[][J], Player* player, Ghost* ghost, int*score, int *life, int *coin_cnt_left)
{
	int i, IsCol = 0;

	if (map[player->i][player->j] == 1)
	{
		map[player->i][player->j] = 0;
		(*coin_cnt_left)--;
		(*score) += 10;
	}
	else if (map[player->i][player->j] == 2)
	{
		player->mode = MODE;
		map[player->i][player->j] = 0;
		(*score) += 50;
	}

	for (i = 0; i < N; i++)
	{
		if (!ghost[i].valid) continue;
		if (ghost[i].valid > 1) 
		{ printf("%d\n", ghost[i].valid);  ghost[i].valid--; if (ghost[i].valid == 1) { ghost[i].i = GHOST_I; ghost[i].j = GHOST_J; ghost[i].i_offset = ghost[i].j_offset = 0; continue; } }
		if ((*player).i == ghost[i].i && (*player).j == ghost[i].j)
		{
			IsCol = 1;
			break;
		}
	}

	if(IsCol)
	{
		/*normal*/
		if (!(*player).mode)
		{
			(*life)--;

			player->i = PLAYER_I;
			player->j = PLAYER_J;

			player->i_offset = player->j_offset = 0;
		}
		/*fellet*/
		else
		{
			ghost[i].valid = 10;

			map[(*player).i][(*player).j] = 0;
			(*score) += 200;
		}
	}



	if (player->mode)player->mode--;
}

int gamecheck(int* life, int* coin_cnt_left)
{
	if (*life == 0) return 1;
	if (*coin_cnt_left == 0) return 2;
	return 0;
}

int proc(int map[][J], Player* player, Ghost* ghost, int level, int*score, int *life, int *coin_cnt_left)
{
	
	offsetupdate(map, player, ghost);
	
	colcheck(map, player,  ghost, score, life, coin_cnt_left);

	return gamecheck(life, coin_cnt_left);
}

void show(int map[][J], Player player, Ghost* ghost, int level, int score, int life)
{
	printf("LEVEL: %d\n", level);
	printf("LIFE : %d\n", life);
	printf("SCORE : %d\n", score);

	for (int i = 0; i < I; i++)
	{
		for (int j = 0; j < J; j++)
		{
			int IsGhost = 0;

			for (int k = 0; k < N; k++)
			{
				if (!ghost[k].valid) continue;
				if (ghost[k].i == i&&ghost[k].j == j)
				{
					if (!player.mode) printf("¡ß");
					else printf("¡Þ");
					IsGhost = 1;
					break;
				}
			}

			if (IsGhost) continue;
			
			if (player.i == i && player.j == j)
				printf("¡Ü");
			else if (map[i][j] > 2)
				printf("¡á");
			else if (map[i][j] == 1)
				printf("¢Á");
			else if (map[i][j] == 2)
				printf("¡Ø");
			/*else if (map[i][j] == 7)
				printf("¢Í");*/
			else
				printf("  ");
		}
		printf("\n");
	}
}

int main()
{
	Player player;
	Ghost ghost[4];
	const int data_map[I][J] = {
		{ 3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,34,35,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,5 },
		{ 10,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,22,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,8 },
		{ 10,0,15,19,19,19,17,0,15,19,19,19,19,19,17,0,15,19,17,0,22,20,0,15,19,17,0,15,19,19,19,19,19,17,0,15,19,19,19,17,0,8 },
		{ 10,0,22,0,0,0,20,0,22,0,0,0,0,0,20,0,22,0,20,0,22,20,0,22,0,20,0,22,0,0,0,0,0,20,0,22,0,0,0,20,0,8 },
		{ 10,0,16,21,21,21,18,0,16,21,21,21,21,21,18,0,16,21,18,0,22,20,0,16,21,18,0,16,21,21,21,21,21,18,0,16,21,21,21,18,0,8 },
		{ 10,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,22,20,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,8 },
		{ 10,0,15,19,19,19,19,19,19,17,0,15,17,0,15,19,19,19,17,0,22,20,0,15,19,19,19,17,0,15,17,0,15,19,19,19,19,19,19,17,0,8 },
		{ 10,0,16,21,21,21,21,21,21,18,0,22,20,0,16,21,21,21,18,0,16,18,0,16,21,21,21,18,0,22,20,0,16,21,21,21,21,21,21,18,0,8 },
		{ 10,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,8 },
		{ 4,9,9,9,9,9,17,0,15,19,19,18,20,0,15,19,19,19,19,19,19,19,19,19,19,19,19,17,0,22,16,19,19,17,0,15,9,9,9,9,9,6 },
		{ 0,0,0,0,0,0,10,0,16,21,21,17,20,0,16,21,21,21,21,21,21,21,21,21,21,21,21,18,0,22,15,21,21,18,0,8,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,10,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,8,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,10,0,15,17,0,22,20,0,15,19,17,0,23,27,27,31,33,33,32,27,27,24,0,22,20,0,15,17,0,8,0,0,0,0,0,0 },
		{ 7,7,7,7,7,7,18,0,22,20,0,16,18,0,22,0,20,0,28,0,0,0,0,0,0,0,0,30,0,16,18,0,22,20,0,16,7,7,7,7,7,7 },
		{ -10,0,0,0,0,0,0,0,22,20,0,0,0,0,22,0,20,0,28,0,0,0,0,0,0,0,0,30,0,0,0,0,22,20,0,0,0,0,0,0,0,-11 },
		{ 9,9,9,9,9,9,17,0,22,16,19,19,17,0,16,21,18,0,25,29,29,29,29,29,29,29,29,26,0,15,19,19,18,20,0,15,9,9,9,9,9,9 },
		{ 0,0,0,0,0,0,10,0,22,15,21,21,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,21,21,17,20,0,8,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,10,0,22,20,0,0,0,0,15,19,17,0,15,19,19,19,19,17,0,15,19,17,0,0,0,0,22,20,0,8,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,10,0,22,20,0,15,17,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,15,17,0,22,20,0,8,0,0,0,0,0,0 },
		{ 3,7,7,7,7,7,18,0,16,18,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,16,18,0,16,7,7,7,7,7,5 },
		{ 10,0,0,0,0,0,0,0,0,0,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,0,0,0,0,0,0,0,0,0,8 },
		{ 10,0,15,19,19,19,19,19,17,0,15,18,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,16,17,0,15,19,19,19,19,19,17,0,8 },
		{ 10,0,16,21,21,21,21,21,18,0,16,17,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,15,18,0,16,21,21,21,21,21,18,0,8 },
		{ 10,0,0,0,0,0,0,0,0,0,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,0,0,0,0,0,0,0,0,0,8 },
		{ 14,19,19,19,19,17,0,15,19,17,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,15,19,17,0,15,19,19,19,19,13 },
		{ 12,21,21,21,21,18,0,22,0,20,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,22,0,20,0,16,21,21,21,21,11 },
		{ 10,0,0,0,0,0,0,22,0,20,0,16,18,0,16,21,18,0,16,21,21,21,21,18,0,16,21,18,0,16,18,0,22,0,20,0,0,0,0,0,0,8 },
		{ 10,0,15,19,19,17,0,22,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,0,20,0,15,19,19,17,0,8 },
		{ 10,0,22,0,0,20,0,22,0,20,0,15,19,19,19,19,19,19,19,19,17,0,15,19,19,19,19,19,19,19,17,0,22,0,20,0,22,0,0,20,0,8 },
		{ 10,0,16,21,21,18,0,16,21,18,0,16,21,21,21,21,21,21,21,21,18,0,16,21,21,21,21,21,21,21,18,0,16,21,18,0,16,21,21,18,0,8 },
		{ 10,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,8 },
		{ 4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,6 }
	};
	int map[I][J];
	int score = 0, life=3;
	int level = 1, coin_cnt_left = 0;
	int check;

	while(1)
	{
		Init(data_map, map, &player, ghost, level, &coin_cnt_left);
		while (1)
		{
			Input(&player);

			check = proc(map, &player, ghost, level, &score, &life, &coin_cnt_left);

			if (check == 1) { printf("GAMEOVER\n"); Sleep(500); life = 3; level = 1; score = 0; break; }
			else if (check == 2) { level++; break; }


			system("cls");
			show(map, player, ghost, level, score, life);
			
			Sleep(33);
			

		
		}
		if (level == 5) { printf("GAMECLEAR\n"); return 0; }
	}
}