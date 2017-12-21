#include "lcd.h"
#include "s3c_timer.h"

#include "pacman.h"
#include "res-pm.h"

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

int main(void){
  initial_env();
  die();
  return 0;
}
