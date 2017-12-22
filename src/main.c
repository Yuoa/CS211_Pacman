#include "lcd.h"
#include "s3c_timer.h"
#include "interrupt.h"
#include "graphics.h"

#include "res-gg.h"
#include "res-item.h"
#include "res-map.h"
#include "res-pm.h"
#include "res-score.h"
#include "res-board.h"

#define WIDTH 21
#define HEIGHT 17

int direction = -1;
int cnt[4] = {0, 0, 0, 0};
int old_x, old_y;

void draw_image(int x, int y, int w, int h, unsigned char *data) {
  unsigned char *phy_addr = FB_ADDR;
  int i, j, k;
  int index = 0;
  for (i = y; i < y+h; i++) {
    for (j = x; j < x+w; j++) {
      for (k = 0; k < 3; k++) {
        *(phy_addr+800*4*i+4*j+k) = *(data+index);
        index++;
      }
      *(phy_addr+800*4*i+4*j+3) = 0x0;
    }
  }
  set_wincon0_enable();
  set_vidcon0_enable();
}


void draw_image_without_blackbg(int x, int y, int w, int h, unsigned char *data) {
  unsigned char *phy_addr = FB_ADDR;
  int i, j, k;
  int index = 0;
  for (i = y; i < y+h; i++) {
    for (j = x; j < x+w; j++) {
      for (k = 0; k < 3; k++) {
	if(*(data+index) != 0)
        	*(phy_addr+800*4*i+4*j+k) = *(data+index);
        index++;
      }
      *(phy_addr+800*4*i+4*j+3) = 0x0;
    }
  }
  set_wincon0_enable();
  set_vidcon0_enable();
}

void draw_image_with_otherbg(int x, int y, int w, int h, unsigned char *data, unsigned char bg) {
  unsigned char *phy_addr = FB_ADDR;
  int i, j, k;
  int index = 0;
  for (i = y; i < y+h; i++) {
    for (j = x; j < x+w; j++) {
      for (k = 0; k < 3; k++) {
	if(*(data+index) != 0)
        	*(phy_addr+800*4*i+4*j+k) = *(data+index);
	else
		*(phy_addr+800*4*i+4*j+k) = bg;
        index++;
      }
      *(phy_addr+800*4*i+4*j+3) = 0x0;
    }
  }
  set_wincon0_enable();
  set_vidcon0_enable();
}

void draw_image_with_color(int x, int y, int w, int h, unsigned char bg) {
  unsigned char *phy_addr = FB_ADDR;
  int i, j, k;
  int index = 0;
  for (i = y; i < y+h; i++) {
    for (j = x; j < x+w; j++) {
      for (k = 0; k < 3; k++) {
	*(phy_addr+800*4*i+4*j+k) = bg;
        index++;
      }
      *(phy_addr+800*4*i+4*j+3) = 0x0;
    }
  }
  set_wincon0_enable();
  set_vidcon0_enable();
}

void die(int pacx, int pacy) {

int befx = 163 + 15*pacx;
int befy = pacy * 15 - 7;
  draw_image(befx, befy, 30, 30, pm_die_0);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_1);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_2);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_3);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_4);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_5);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_6);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_7);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_8);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_9);
  mdelay(100);
  draw_image(befx, befy, 30, 30, pm_die_a);
  mdelay(100);
  draw_image_with_color(befx, befy, 30, 30, 0);
}

void initial_env(void) {
  disable_interrupts();
  send_x = 0;
  send_y = 0;
  lcd_init();
  gfx_init();
  mango_interrupt_init();
  enable_interrupts();
}

void change_dir(void) {
  int sub[4] = {0, 0, 0, 0};
  int max, index;
  int i;

  sub[0] = send_x - old_x;
  sub[1] = send_y - old_y;
  sub[2] = -sub[0];
  sub[3] = -sub[1];
  max = sub[0];
  index = 0;
  for (i = 1; i < 4; i++) {
    if (sub[i] > max) {
      index = i;
      max = sub[i];
    }
  }
  if (max < 3) index = -1;
  for (i = 0; i < 4; i++) {
    if (index != i) cnt[i] = 0;
  }
  if (index != -1) {
    cnt[index]++;
    if (cnt[index] >= 3) direction = index;
  }
}

/*void draw_map(int map[][WIDTH]) {
  int i, j;

  draw_image(0, 0, 15, 15, map_cd_left_top);
  draw_image(0, 465, 15, 15, map_cd_left_bottom);
  draw_image(585, 0, 15, 15, map_cd_right_top);
  draw_image(585, 465, 15, 15, map_cd_right_bottom);

  for (j = 1; j < HEIGHT-1; j++) {
    if (map[j][1] != 1) {
      draw_image(0, 30*j-15, 15, 15, map_rd_left);
      draw_image(0, 30*j, 15, 15, map_rd_left);
    }
    else {
      draw_image(0, 30*j-15, 15, 15, map_cd_left_lower);
      draw_image(0, 30*j, 15, 15, map_cd_left_upper);
    }
    if (map[j][WIDTH-2] != 1) {
      draw_image(585, 30*j-15, 15, 15, map_rd_right);
      draw_image(585, 30*j, 15, 15, map_rd_right);
    }
    else {
      draw_image(585, 30*j-15, 15, 15, map_cd_right_lower);
      draw_image(585, 30*j, 15, 15, map_cd_right_upper);
    }
  }

  for (i = 1; i < WIDTH-1; i++) {
    if (map[1][i] != 1) {
      draw_image(30*i-15, 0, 15, 15, map_cd_top);
      draw_image(30*i, 0, 15, 15, map_cd_top);
    }
    else {
      draw_image(30*i-15, 0, 15, 15, map_cd_upper_right);
      draw_image(30*i, 0, 15, 15, map_cd_upper_left);
    }
    draw_image(30*i-15, 465, 15, 15, map_cd_bottom);
    draw_image(30*i, 465, 15, 15, map_cd_bottom);
  }



  for (j = 1; j < HEIGHT-1; j++) {
    for (i = 1; i < WIDTH-1; i++) {
      if (map[j][i] != 1) {
        draw_image(30*i-15, 30*j-15, 15, 15, map_blank);
        draw_image(30*i, 30*j-15, 15, 15, map_blank);
        draw_image(30*i-15, 30*j, 15, 15, map_blank);
        draw_image(30*i-15, 30*j-15, 15, 15, map_blank);
      }
      else if (map[j][i-1] == 1) { // left
        if (map[j][i+1] == 1) { // right
          if (map[j-1][i] == 1) { // up
            if (map[j+1][i] == 1) { // down
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_right_bottom);
              draw_image(30*i, 30*j-15, 15, 15, map_c_left_bottom);
              draw_image(30*i-15, 30*j, 15, 15, map_c_right_top);
              draw_image(30*i, 30*j, 15, 15, map_c_left_top);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_right_bottom);
              draw_image(30*i, 30*j-15, 15, 15, map_c_left_bottom);
              draw_image(30*i-15, 30*j, 15, 15, map_c_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_bottom);
            }
          }
          else {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_right_top);
              draw_image(30*i, 30*j, 15, 15, map_c_left_top);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_bottom);
            }
          }
        }
        else {
          if (map[j-1][i] == 1) {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_right_bottom);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right);
              draw_image(30*i-15, 30*j, 15, 15, map_c_right_top);
              draw_image(30*i, 30*j, 15, 15, map_c_right);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_right_bottom);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right);
              draw_image(30*i-15, 30*j, 15, 15, map_c_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_right_bottom);
            }
          }
          else {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_right_top);
              draw_image(30*i, 30*j, 15, 15, map_c_right);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_right_bottom);
            }
          }
        }
      }
      else {
        if (map[j][i+1] == 1) {
          if (map[j-1][i] == 1) {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left);
              draw_image(30*i, 30*j-15, 15, 15, map_c_left_bottom);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left);
              draw_image(30*i, 30*j, 15, 15, map_c_left_top);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left);
              draw_image(30*i, 30*j-15, 15, 15, map_c_left_bottom);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_bottom);
            }
          }
          else {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left);
              draw_image(30*i, 30*j, 15, 15, map_c_left_top);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_bottom);
            }
          }
        }
        else {
          if (map[j-1][i] == 1) {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left);
              draw_image(30*i, 30*j, 15, 15, map_c_right);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_right_bottom);
            }
          }
          else {
            if (map[j+1][i] == 1) {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left);
              draw_image(30*i, 30*j, 15, 15, map_c_right);
            }
            else {
              draw_image(30*i-15, 30*j-15, 15, 15, map_c_left_top);
              draw_image(30*i, 30*j-15, 15, 15, map_c_right_top);
              draw_image(30*i-15, 30*j, 15, 15, map_c_left_bottom);
              draw_image(30*i, 30*j, 15, 15, map_c_right_bottom);
            }
          }
        }
      }
    }
  }
}*/

void movegg_red(int pacx, int pacy, int dir) {

	static int befx = 0, befy = 0;
	befx = 163 + 15*pacx;
	befy = pacy * 15 - 7;

	switch(dir) {

	case 0: //right
		draw_image(befx, befy, 30, 30, gg_blinky_right_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_blinky_right_1);
		mdelay(50);
		break;
	case 1: //down
		draw_image(befx, befy, 30, 30, gg_blinky_down_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_blinky_down_1);
		mdelay(50);
		break;
	case 2: //left
		draw_image(befx, befy, 30, 30, gg_blinky_left_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_blinky_left_1);
		mdelay(50);
		break;
	case 3: //up
		draw_image(befx, befy, 30, 30, gg_blinky_up_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_blinky_up_1);
		mdelay(50);
		break;

	}	
	

}

void movegg_pink(int pacx, int pacy, int dir) {

	static int befx = 0, befy = 0;
	befx = 163 + 15*pacx;
	befy = pacy * 15 - 7;

	switch(dir) {

	case 0: //right
		draw_image(befx, befy, 30, 30, gg_pinky_right_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_pinky_right_1);
		mdelay(50);
		break;
	case 1: //down
		draw_image(befx, befy, 30, 30, gg_pinky_down_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_pinky_down_1);
		mdelay(50);
		break;
	case 2: //left
		draw_image(befx, befy, 30, 30, gg_pinky_left_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_pinky_left_1);
		mdelay(50);
		break;
	case 3: //up
		draw_image(befx, befy, 30, 30, gg_pinky_up_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_pinky_up_1);
		mdelay(50);
		break;

	}	
	

}


void movegg_blue(int pacx, int pacy, int dir) {

	static int befx = 0, befy = 0;
	befx = 163 + 15*pacx;
	befy = pacy * 15 - 7;

	switch(dir) {

	case 0: //right
		draw_image(befx, befy, 30, 30, gg_inky_right_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_inky_right_1);
		mdelay(50);
		break;
	case 1: //down
		draw_image(befx, befy, 30, 30, gg_inky_down_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_inky_down_1);
		mdelay(50);
		break;
	case 2: //left
		draw_image(befx, befy, 30, 30, gg_inky_left_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_inky_left_1);
		mdelay(50);
		break;
	case 3: //up
		draw_image(befx, befy, 30, 30, gg_inky_up_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_inky_up_1);
		mdelay(50);
		break;

	}	
	

}

void movegg_yellow(int pacx, int pacy, int dir) {

	static int befx = 0, befy = 0;
	befx = 163 + 15*pacx;
	befy = pacy * 15 - 7;

	switch(dir) {

	case 0: //right
		draw_image(befx, befy, 30, 30, gg_clyde_right_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_clyde_right_1);
		mdelay(50);
		break;
	case 1: //down
		draw_image(befx, befy, 30, 30, gg_clyde_down_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_clyde_down_1);
		mdelay(50);
		break;
	case 2: //left
		draw_image(befx, befy, 30, 30, gg_clyde_left_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_clyde_left_1);
		mdelay(50);
		break;
	case 3: //up
		draw_image(befx, befy, 30, 30, gg_clyde_up_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, gg_clyde_up_1);
		mdelay(50);
		break;

	}	
	

}

void moveprint_pacman(int pacx, int pacy, int dir) {

	static int befx = 0, befy = 0;
	befx = 163 + 15*pacx;
	befy = pacy * 15 - 7;

	switch(dir) {

	case 0: //right
		draw_image(befx, befy, 30, 30, pm_right_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_right_1);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_right_2);
		mdelay(50);
		break;
	case 1: //down
		draw_image(befx, befy, 30, 30, pm_down_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_down_1);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_down_2);
		mdelay(50);
		break;
	case 2: //left
		draw_image(befx, befy, 30, 30, pm_left_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_left_1);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_left_2);
		mdelay(50);
		break;
	case 3: //up
		draw_image(befx, befy, 30, 30, pm_up_0);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_up_1);
		mdelay(50);
		draw_image(befx, befy, 30, 30, pm_up_2);
		mdelay(50);
		break;

	}	
	

}

void updatescoredisp(int score) {

  static unsigned char* numlist[10] = {board_0, board_1, board_2, board_3, board_4, board_5, board_6, board_7, board_8, board_9};

  static int dish = 45;

  //draw_image(25, dish, 120, 15, board_background);
  //printf("score=%d\n", score);

  if(score>=10000000) {
	draw_image_with_otherbg(25, dish, 15, 15, numlist[score / 10000000], 20);
	score %= 10000000;
	draw_image_with_otherbg(40, dish, 15, 15, numlist[score / 1000000], 20);
	score %= 1000000;
	draw_image_with_otherbg(55, dish, 15, 15, numlist[score / 100000], 20);
	score %= 100000;
	draw_image_with_otherbg(70, dish, 15, 15, numlist[score / 10000], 20);
	score %= 10000;
	draw_image_with_otherbg(85, dish, 15, 15, numlist[score / 1000], 20);
	score %= 1000;
	draw_image_with_otherbg(100, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(115, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(130, dish, 15, 15, numlist[score], 20);
  } else if (score >=1000000) {
	draw_image_with_otherbg(32, dish, 15, 15, numlist[score / 1000000], 20);
	score %= 1000000;
	draw_image_with_otherbg(57, dish, 15, 15, numlist[score / 100000], 20);
	score %= 100000;
	draw_image_with_otherbg(62, dish, 15, 15, numlist[score / 10000], 20);
	score %= 10000;
	draw_image_with_otherbg(77, dish, 15, 15, numlist[score / 1000], 20);
	score %= 1000;
	draw_image_with_otherbg(92, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(107, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(122, dish, 15, 15, numlist[score], 20);
  } else if (score >=100000) {
	draw_image_with_otherbg(40, dish, 15, 15, numlist[score / 100000], 20);
	score %= 100000;
	draw_image_with_otherbg(55, dish, 15, 15, numlist[score / 10000], 20);
	score %= 10000;
	draw_image_with_otherbg(70, dish, 15, 15, numlist[score / 1000], 20);
	score %= 1000;
	draw_image_with_otherbg(85, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(100, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(115, dish, 15, 15, numlist[score], 20);
  } else if (score >= 10000) {
	draw_image_with_otherbg(47, dish, 15, 15, numlist[score / 10000], 20);
	score %= 10000;
	draw_image_with_otherbg(62, dish, 15, 15, numlist[score / 1000], 20);
	score %= 1000;
	draw_image_with_otherbg(77, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(92, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(107, dish, 15, 15, numlist[score], 20);
  } else if (score >=1000) {
	draw_image_with_otherbg(55, dish, 15, 15, numlist[score / 1000], 20);
	score %= 1000;
	draw_image_with_otherbg(70, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(85, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(100, dish, 15, 15, numlist[score], 20);
  } else if (score >=100) {
	draw_image_with_otherbg(62, dish, 15, 15, numlist[score / 100], 20);
	score %= 100;
	draw_image_with_otherbg(77, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(92, dish, 15, 15, numlist[score], 20);
  } else if (score >=10) {
	draw_image_with_otherbg(70, dish, 15, 15, numlist[score / 10], 20);
	score %= 10;
	draw_image_with_otherbg(85, dish, 15, 15, numlist[score], 20);
  } else {
	draw_image_with_otherbg(77, dish, 15, 15, numlist[score], 20);
  }

}

int main(void){

  initial_env();

/*int test_map[17][21] = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1}, 
{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1}, 
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, 
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

draw_map(test_map);*/

unsigned char* dmap[36] = {

  map_blank,//0
  map_pacdot,//1
  map_powerpellet,//2
  map_cd_left_top,//3
  map_cd_left_bottom,//4
  map_cd_right_top,//5
  map_cd_right_bottom,//6
  map_cd_top,//7
  map_rd_right,//8
  map_cd_bottom,//9
  map_rd_left,//10
  map_cd_right_upper,//11
  map_cd_left_upper,//12
  map_cd_right_lower,//13
  map_cd_left_lower,//14
  map_c_left_top,//15
  map_c_left_bottom,//16
  map_c_right_top,//17
  map_c_right_bottom,//18
  map_c_top,//19
  map_c_right,//20
  map_c_bottom,//21
  map_c_left,//22
  map_rd_left_top,//23
  map_rd_right_top,//24
  map_rd_left_bottom,//25
  map_rd_right_bottom,//26
  map_rd_top,//27
  map_rd_right,//28
  map_rd_bottom,//29
  map_rd_left,//30
  map_rd_door_left,//31
  map_rd_door_right,//32
  map_rd_door,//33
  map_cd_upper_right,//34
  map_cd_upper_left//35

};

static int unmap[32][42] = {
  /*00*/{3,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,34,35,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,5},
  /*01*/{10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
  /*02*/{10,0,15,19,19,19,17,0,15,19,19,19,19,19,17,0,15,19,17,0,22,20,0,15,19,17,0,15,19,19,19,19,19,17,0,15,19,19,19,17,0,8},
  /*03*/{10,0,22,0,0,0,20,0,22,0,0,0,0,0,20,0,22,0,20,0,22,20,0,22,0,20,0,22,0,0,0,0,0,20,0,22,0,0,0,20,0,8},
  /*04*/{10,0,16,21,21,21,18,0,16,21,21,21,21,21,18,0,16,21,18,0,22,20,0,16,21,18,0,16,21,21,21,21,21,18,0,16,21,21,21,18,0,8},
  /*05*/{10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
  /*06*/{10,0,15,19,19,19,19,19,19,17,0,15,17,0,15,19,19,19,17,0,22,20,0,15,19,19,19,17,0,15,17,0,15,19,19,19,19,19,19,17,0,8},
  /*07*/{10,0,16,21,21,21,21,21,21,18,0,22,20,0,16,21,21,21,18,0,16,18,0,16,21,21,21,18,0,22,20,0,16,21,21,21,21,21,21,18,0,8},
  /*08*/{10,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,8},
  /*09*/{4,9,9,9,9,9,17,0,15,19,19,18,20,0,15,19,19,19,19,19,19,19,19,19,19,19,19,17,0,22,16,19,19,17,0,15,9,9,9,9,9,6},
  /*10*/{0,0,0,0,0,0,10,0,16,21,21,17,20,0,16,21,21,21,21,21,21,21,21,21,21,21,21,18,0,22,15,21,21,18,0,8,0,0,0,0,0,0},
  /*11*/{0,0,0,0,0,0,10,0,0,0,0,22,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,20,0,0,0,0,8,0,0,0,0,0,0},
  /*12*/{0,0,0,0,0,0,10,0,15,17,0,22,20,0,15,19,17,0,23,27,27,31,33,33,32,27,27,24,0,22,20,0,15,17,0,8,0,0,0,0,0,0},
  /*13*/{7,7,7,7,7,7,18,0,22,20,0,16,18,0,22,0,20,0,28,0,0,0,0,0,0,0,0,30,0,16,18,0,22,20,0,16,7,7,7,7,7,7},
  /*14*/{0,0,0,0,0,0,0,0,22,20,0,0,0,0,22,0,20,0,28,0,0,0,0,0,0,0,0,30,0,0,0,0,22,20,0,0,0,0,0,0,0,0},
  /*15*/{9,9,9,9,9,9,17,0,22,16,19,19,17,0,16,21,18,0,25,29,29,29,29,29,29,29,29,26,0,15,19,19,18,20,0,15,9,9,9,9,9,9},
  /*16*/{0,0,0,0,0,0,10,0,22,15,21,21,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,21,21,17,20,0,8,0,0,0,0,0,0},
  /*17*/{0,0,0,0,0,0,10,0,22,20,0,0,0,0,15,19,17,0,15,19,19,19,19,17,0,15,19,17,0,0,0,0,22,20,0,8,0,0,0,0,0,0},
  /*18*/{0,0,0,0,0,0,10,0,22,20,0,15,17,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,15,17,0,22,20,0,8,0,0,0,0,0,0},
  /*19*/{3,7,7,7,7,7,18,0,16,18,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,16,18,0,16,7,7,7,7,7,5},
  /*20*/{10,0,0,0,0,0,0,0,0,0,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,0,0,0,0,0,0,0,0,0,8},
  /*21*/{10,0,15,19,19,19,19,19,17,0,15,18,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,16,17,0,15,19,19,19,19,19,17,0,8},
  /*22*/{10,0,16,21,21,21,21,21,18,0,16,17,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,15,18,0,16,21,21,21,21,21,18,0,8},
  /*23*/{10,0,0,0,0,0,0,0,0,0,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,0,0,0,0,0,0,0,0,0,8},
  /*24*/{14,19,19,19,19,17,0,15,19,17,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,15,19,17,0,15,19,19,19,19,13},
  /*25*/{12,21,21,21,21,18,0,22,0,20,0,22,20,0,22,0,20,0,22,0,0,0,0,20,0,22,0,20,0,22,20,0,22,0,20,0,16,21,21,21,21,11},
  /*26*/{10,0,0,0,0,0,0,22,0,20,0,16,18,0,16,21,18,0,16,21,21,21,21,18,0,16,21,18,0,16,18,0,22,0,20,0,0,0,0,0,0,8},
  /*27*/{10,0,15,19,19,17,0,22,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,0,20,0,15,19,19,17,0,8},
  /*28*/{10,0,22,0,0,20,0,22,0,20,0,15,19,19,19,19,19,19,19,19,17,0,15,19,19,19,19,19,19,19,17,0,22,0,20,0,22,0,0,20,0,8},
  /*29*/{10,0,16,21,21,18,0,16,21,18,0,16,21,21,21,21,21,21,21,21,18,0,16,21,21,21,21,21,21,21,18,0,16,21,18,0,16,21,21,18,0,8},
  /*30*/{10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8},
  /*31*/{4,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,6}
};

int xi, xj;

draw_image_with_color(0,0,170,480,20);
draw_image_without_blackbg(47,27,15,15,board_S);
draw_image_without_blackbg(62,27,15,15,board_C);
draw_image_without_blackbg(77,27,15,15,board_O);
draw_image_without_blackbg(92,27,15,15,board_R);
draw_image_without_blackbg(107,27,15,15,board_E);

int dispScore = 0;

for(xi = 0;xi < 32; xi++)
  for(xj = 0;xj < 42; xj++)
    draw_image(170 + 15*xj, 15 * xi, 15, 15, dmap[map[xi][xj]]);

draw_image_without_blackbg(440,450,15,15,board_yR);
draw_image_without_blackbg(455,450,15,15,board_yE);
draw_image_without_blackbg(470,450,15,15,board_yA);
draw_image_without_blackbg(485,450,15,15,board_yD);
draw_image_without_blackbg(500,450,15,15,board_yY);
draw_image_without_blackbg(515,450,15,15,board_yimp);

int tempdir = -1;

int pacx = 21, pacy = 28;

  //die();
  unsigned long cnt = 0;
  while (1) {
    old_x = send_x;
    old_y = send_y;
    mdelay(10);
    tempdir = direction;
    change_dir();
    if(tempdir == -1 && direction != tempdir)
      draw_image_with_color(440,450,90,15,0);

dispScore+=1;

    if(cnt++ % 10 == 0) {

	updatescoredisp(dispScore);

	moveprint_pacman(pacx, pacy, direction);
	//movegg_red(

    }

    if(dispScore > 5000) {
	die(pacx, pacy);
	break;
    }

  }
  return 0;
}
