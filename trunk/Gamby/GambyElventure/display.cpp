#include <avr/pgmspace.h>
#include <Gamby.h>
#include "item_bitmap.h"
#include "map_bitmap.h"
#include "elf.h"
#include "display.h"
#include "room.h"

extern GambyGraphicsMode gamby;

#define SIZEOF_DISPLAY_RECORD 6

void drawDisplay(Elf elf)
{
  
  //blank out all the sprites (since they could be in the buffer)
  for (char t=0; t < 11; t++)
  {
     for (char u=0; u < 7; u++)
	 {
	    gamby.drawSprite(t*8, u*8, map_bitmap);
	 }
  }
  gamby.clearScreen();
  gamby.update();  
  
  //draw hearts
  char x_offset = 1;
  for (char i=1; i <= MAX_HEARTS; i++)
  {
    if (i <= elf.hearts)
    {
      //draw hearts
      gamby.drawSprite(x_offset, 4, item_bitmap, 2);
    } else {
      //draw blanks
      gamby.drawSprite(x_offset, 4, item_bitmap, 3);
    }	
    x_offset += 8;
	
  }
   
  //draw crystals (inventory)
  x_offset = 1;
  for (char j=0; j < MAX_ITEMS; j++)
  {
    switch (elf.items[j])
    {
	  case ITEM_CRYSTAL:
        //draw crystals
        gamby.drawSprite(x_offset, 12, item_bitmap, 4);
		break;
		
	  case ITEM_ARMOR:
        //draw crystals
        gamby.drawSprite(x_offset, 12, item_bitmap, 10);
		break;

	  case ITEM_ORB:
        //draw crystals
        gamby.drawSprite(x_offset, 12, item_bitmap, 8);
		break;

	  case ITEM_STAFF:
        //draw crystals
        gamby.drawSprite(x_offset, 12, item_bitmap, 12);
		break;
		
      default:
        //draw blanks
        gamby.drawSprite(x_offset, 12, map_bitmap);
        break;
    }	
    x_offset += 8;
    
    gamby.update();
  }   
}

