#include "lcd.h"
#include "s3c_timer.h"

#include "pacman.h"

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

int main(void){
  initial_env();
  
  int i;
  for (i = 0; i < 100; i++) {
    draw_image(200+i, 0, 224, 16, pacman_part);
    mdelay(10);
  }
  return 0;
}
