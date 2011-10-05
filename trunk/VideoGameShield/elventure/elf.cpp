#include <avr/pgmspace.h>
#include <TVout.h>
#include "elf.h"
#include "elf_bitmap.h"
#include "map_bitmap.h"
#include "map.h"
#include "room.h"
#include "item.h"
#include "display.h"
extern TVout TV;

#define FACING_DOWN  0
#define FACING_UP    3
#define FACING_LEFT  6
#define FACING_RIGHT 9

#define STEP_LENGTH  4

#define SIZEOF_ELF_RECORD 10

Elf elf = {FACING_DOWN, 1, 36, 24, 3, 0};

void showElf()
{
  moveElf(elf.facing);
}

void moveElf(unsigned char facing)
{
  //erase the old elf image (using blank map tile)
  TV.bitmap(elf.x, elf.y, map_bitmap);
  TV.bitmap(elf.x, elf.y + 8, map_bitmap);
  
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
             if (checkMapRoomMove(elf.x, elf.y + 16) == 0) 
	        if (checkMapRoomMove(elf.x+4, elf.y + 16) == 0) elf.y += STEP_LENGTH;
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
             if (checkMapRoomMove(elf.x, elf.y - 4) == 0)
	        if (checkMapRoomMove(elf.x + 4, elf.y - 4) == 0) elf.y -= STEP_LENGTH;
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
                if (checkMapRoomMove(elf.x - 4, elf.y + 12) == 0) elf.x -= STEP_LENGTH;
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
               if (checkMapRoomMove(elf.x + 12, elf.y + 12) == 0) elf.x += STEP_LENGTH;	
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

void throwSword()
{
  //retrieve the sword room element
  RoomElement element = getRoomElement(0);

  //if it is already active, do nothing, otherwise
  //initiate the sword being thrown
  if (element.state == STATE_HIDDEN)
  {
	  switch (elf.facing)
	  {
		case FACING_DOWN:
			element.state = STATE_MOVE_DOWN;
			element.x = elf.x;
			element.y = elf.y + 16;
			break;
			
		case FACING_UP:
			element.state = STATE_MOVE_UP;
			element.x = elf.x;
			element.y = elf.y - 8;			
			break;

		case FACING_LEFT:
			element.state = STATE_MOVE_LEFT;
			element.x = elf.x - 8;
			element.y = elf.y;				
			break;
		
		case FACING_RIGHT:
			element.state = STATE_MOVE_RIGHT;
			element.x = elf.x + 8;
			element.y = elf.y;	
			break;
	  }  
	  updateRoomElement(element);
  }
}

RoomElement hitElf(RoomElement element)
{
  //hit by a monster
  if (element.type < 50)
  {
    //check the counter, so hearts are not 
	//removed unless the monster has not been 'hit'
	//already
    if (element.counter == 0)
	{
      element.counter = COUNTER_START;
      elf.hearts--;
      if (elf.hearts < 1)
      {
        //game over
      }
	}
	
	//when the elf and a monster 'bump,' move the monster
	//in the opposite direction
	switch (element.state)
	{
	  case STATE_MOVE_UP:
	     element.state = STATE_MOVE_DOWN;
		 break;
		 
	  case STATE_MOVE_DOWN:
	     element.state = STATE_MOVE_UP;
		 break;

	  case STATE_MOVE_LEFT:
	     element.state = STATE_MOVE_RIGHT;
		 break;		 

	  case STATE_MOVE_RIGHT:
	     element.state = STATE_MOVE_LEFT;
		 break;	 
	}
  } else {
	//if it is a heart, add one to the hearts
	if (element.type == ITEM_HEART)
	{
	  if (elf.hearts < MAX_HEARTS) elf.hearts++;
	  //handle the rest of the item hit
	  element = hitItem(element);	  
	} 
  }
  
  //update the display
  updateDisplay(elf);
  
  return element;
}
  
Elf getElf()
{
    return elf;
}




