#include <avr/pgmspace.h>
#include <TVout.h>
#include "elf.h"
#include "elf_bitmap.h"
#include "map_bitmap.h"
#include "map.h"
extern TVout TV;

#define FACING_DOWN  0
#define FACING_UP    3
#define FACING_LEFT  6
#define FACING_RIGHT 9

#define STEP_LENGTH  4

#define SIZEOF_ELF_RECORD 10

struct Elf
{
  unsigned char facing;
  unsigned char step;
  unsigned char x;
  unsigned char y;
};

Elf elf = {FACING_DOWN, 1, 36, 24};

void showElf()
{
  moveElf(elf.facing);
}

void moveElf(unsigned char facing)
{
  //erase the old elf image (using blank map tile)
  TV.bitmap(elf.x, elf.y, map_bitmap);
  TV.bitmap(elf.x, elf.y+8, map_bitmap);
  
  //if it is a new facing, then reset the step
  if (facing != elf.facing)
  {
     elf.step = 1;
  } else {
     elf.step++;
	 if (elf.step > 2) elf.step = 1;
  }
  
  elf.facing = facing;
  
  switch (facing)
  {
     case FACING_DOWN:
	   if (elf.y < 48)
           {
             if (checkMapRoomMove(elf.x, elf.y + 16) == 0) elf.y += STEP_LENGTH;
           } else {
	     scrollMap(SCROLL_DOWN);
    	     elf.x = 36;
             elf.y = 8;
             elf.facing = FACING_DOWN;
	   }
	   break;
		
	case FACING_UP:
           if (elf.y > 4)
           {
             if (checkMapRoomMove(elf.x, elf.y - 4) == 0) elf.y -= STEP_LENGTH;
           } else {
	     scrollMap(SCROLL_UP);
	     elf.x = 36;
	     elf.y = 32;
	     elf.facing = FACING_UP;
	   }
	   break;
	   
	case FACING_LEFT:
	   if (elf.x > 4)
           {
             if (checkMapRoomMove(elf.x - 4, elf.y) == 0)
                if (checkMapRoomMove(elf.x - 4, elf.y + 8) == 0) elf.x -= STEP_LENGTH;
           } else {
	       scrollMap(SCROLL_LEFT);
	       elf.x = 64;
	       elf.y = 24;
	       elf.facing = FACING_LEFT;
	   }
           break;

        case FACING_RIGHT:
           if (elf.x < 80)
           {
             if (checkMapRoomMove(elf.x + 12, elf.y) == 0)
               if (checkMapRoomMove(elf.x + 12, elf.y + 8) == 0) elf.x += STEP_LENGTH;	
           } else {
	       scrollMap(SCROLL_RIGHT);
	       elf.x = 16;
	       elf.y = 24;
	       elf.facing = FACING_RIGHT;
	   }
           break;
  }
  
  //draw new elf bitmap
  TV.bitmap(elf.x, elf.y, elf_bitmap + ( elf.facing * SIZEOF_ELF_RECORD));
  TV.bitmap(elf.x, elf.y+8, elf_bitmap + ((elf.facing + elf.step) * SIZEOF_ELF_RECORD)); 
}



