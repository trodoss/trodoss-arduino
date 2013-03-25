#include <avr/pgmspace.h>
#include <Gamby.h>
#include "logo_bitmap.h"
extern GambyGraphicsMode gamby;

#define SIZEOF_LOGO_RECORD 10

void drawLogo(char start)
{
  //draw hearts
  char x_offset = 32;
  for (char i=start; i < (start+4); i++)
  {
      //draw logo element
	  gamby.setPos(x_offset, 2);
      gamby.drawIcon(logo_bitmap, i);
   
    x_offset += 8;
  }   
}

