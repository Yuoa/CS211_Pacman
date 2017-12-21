#include "lcd.h"
#include "s3c_timer.h"
#include "interrupt.h"
#include "graphics.h"

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

int main(void){
  initial_env();
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
