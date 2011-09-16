#include <avr/pgmspace.h>
#include <TVout.h>
#include "monster.h"
#include "map.h"
#include "map_bitmap.h"
#include "monster_bitmap.h"
#include "room.h"

extern TVout TV;

#define STEP_LENGTH  4
#define SIZEOF_MONSTER_RECORD 10

char changeMonsterDirection()
{
  return ((random() % 4) + STATE_MOVE_UP);
}

RoomElement moveMonster(RoomElement element)
{
  //erase the old monster image (using blank map tile)
  TV.bitmap(element.x, element.y, map_bitmap);
  TV.bitmap(element.x, element.y+8, map_bitmap);
  
  element.step++;
  if (element.step > 2) element.step = 1;
   
  switch (element.state)
  {
     case STATE_VISIBLE:
	    element.state = changeMonsterDirection();
		break;
	 
	 case STATE_MOVE_UP:
	   if (element.y < 48)
       {
          if (checkMapRoomMove(element.x, element.y + 16) == 0)
	      {
	        element.y += STEP_LENGTH;
	      } else {
	        element.state = changeMonsterDirection();
	      }
	   } else {
	     element.state = changeMonsterDirection();
	   }
	   break;
		
	case STATE_MOVE_DOWN:
           if (element.y > 4)
           {
             if (checkMapRoomMove(element.x, element.y - 4) == 0)
	     {
	       element.y -= STEP_LENGTH;
	     } else {
	       element.state = changeMonsterDirection();
	     }
           } else {
   	    element.state = changeMonsterDirection();
	   }
	   break;

	case STATE_MOVE_LEFT:
           if (element.x > 4)
           {
             if (checkMapRoomMove(element.x - 4, element.y) == 0)
	     {
                if (checkMapRoomMove(element.x - 4, element.y + 8) == 0) 
		{
		  element.x -= STEP_LENGTH;
		} else {
		    element.state = changeMonsterDirection();
	        }
	     } else {
	     element.state = changeMonsterDirection();
	    }
	   } else {
             element.state = changeMonsterDirection();
           }
	   break;	   
	   
	case STATE_MOVE_RIGHT:
           if (element.x < 80)
           {
             if (checkMapRoomMove(element.x + 12, element.y) == 0)
	     {
                if (checkMapRoomMove(element.x + 12, element.y + 8) == 0)
		{
		  element.x -= STEP_LENGTH;
   	        } else {
		    element.state = changeMonsterDirection();
		}
  	     } else {
   	       element.state = changeMonsterDirection();
	     }
	   } else {
             element.state = changeMonsterDirection();
           }
	   break;		      
  }

  
  //draw new monster bitmap
  TV.bitmap(element.x, element.y, monster_bitmap + ( element.type * SIZEOF_MONSTER_RECORD));
  TV.bitmap(element.x, element.y+8, monster_bitmap + ((element.type + element.step) * SIZEOF_MONSTER_RECORD)); 
  
  return element;
}



