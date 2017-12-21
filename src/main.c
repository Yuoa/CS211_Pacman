#include "lcd.h"
#include "s3c_timer.h"
#include "interrupt.h"
#include "graphics.h"

#include "res-gg.h"
#include "res-item.h"
#include "res-map.h"
#include "res-pm.h"
#include "res-score.h"

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

void die(void) {
  draw_image(200, 200, 30, 30, pm_die_0);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_1);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_2);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_3);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_4);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_5);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_6);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_7);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_8);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_9);
  mdelay(100);
  draw_image(200, 200, 30, 30, pm_die_a);
  mdelay(100);
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



  die();
  while (1) {
    old_x = send_x;
    old_y = send_y;
    mdelay(16);
    change_dir();
    printf("%d\n", direction);
  }
  return 0;
}
