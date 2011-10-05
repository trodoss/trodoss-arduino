#include <avr/pgmspace.h>
#include <TVout.h>
#include "display_bitmap.h"
#include "elf.h"
#include "display.h"

extern TVout TV;

#define SIZEOF_DISPLAY_RECORD 6

void updateDisplay(Elf elf)
{
  char x_offset = 98;
  for (char i=1; i <= MAX_HEARTS; i++)
  {
    if (i <= elf.hearts)
    {
      //draw hearts
      TV.bitmap(x_offset, 4, display_bitmap + SIZEOF_DISPLAY_RECORD);
    } else {
      //draw blanks
      TV.bitmap(x_offset, 4, display_bitmap);
    }	
    x_offset += 5;
  }  
}

