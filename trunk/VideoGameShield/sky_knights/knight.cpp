#include <avr/pgmspace.h>
#include <TVout.h>
#include "knight.h"
#include "knight_bitmap.h"
#include "bitmap_funcs.h"
#include "level.h"

extern TVout TV;

#define MAX_VELOCITY 3
#define STEP_LENGTH 2
#define SIZEOF_KNIGHT_RECORD 14

char h_velocity = 0;
char y_velocity = 0;
char x = 60;
char y = 71;
char facing = FACING_LEFT;
char frame = 0;
char state = KNIGHTSTATE_PLAYING;
bool flying = false;
bool breaking = false;

char count = 0;

void knight_draw()
{
  
  //(gravity effect) - check space undearneath knight; if it is 0, then knight is falling (flying)
  if (!flying)
  {
    if (facing == FACING_LEFT) {
	    if (level_check_move(x+5, y + 13) == 0) flying = true;
	} else {
	    if (level_check_move(x,y + 13) == 0) flying = true;
	}
  } 
  
  //handle movement/animation
  if ((count >= MAX_VELOCITY - h_velocity) && (h_velocity > 0) || (flying))
  {
     count = 0;
     eraseBitmapRect(x, y, knight_bitmap);
  
     if (!breaking)
     {

       frame++; 
       if (flying)
       {
	     if ((facing == FACING_LEFT) && (frame > 6)) frame = KNIGHT_FLY_LEFT;
	     if ((facing == FACING_RIGHT) && (frame > 13)) frame = KNIGHT_FLY_RIGHT;
	   } else {
	     if ((facing == FACING_LEFT) && (frame > 3)) frame = KNIGHT_MOVE_LEFT;
	     if ((facing == FACING_RIGHT) && (frame > 10)) frame = KNIGHT_MOVE_RIGHT;	  
	   }
	
	   if (facing == FACING_LEFT)
	   {
	     if (x > h_velocity)
	     {
		  //check to see if we hit anything in the move
		  if (level_check_move_full(x-h_velocity, y) == 0) {
		    //if not, keep on moving
                        x -= h_velocity;
		  } else {
		    //hit something, so stop horizontal velocity
			h_velocity = 0;
		  }
	     } else {
		  //check to see if we hit anything in the move
	      if (level_check_move_full(x-h_velocity, y) == 0) {
                        x = (120 - h_velocity);
		  } else {
			h_velocity = 0;
		  }
	     }
       } else {
	     if (x < (120 -h_velocity))
	     {
		  if (level_check_move_full(x+h_velocity, y) == 0) {
	         x += h_velocity;
		  } else {
		    h_velocity = 0;
		  }
	     } else {
		   if (level_check_move_full(0, y) == 0) {
	         x = 0;
		   } else {
		     h_velocity = 0;
		   }
	     }
       }
	  
	   if (flying)
	   {
	     if (y_velocity == 0)
	     {
		  if (y < 80)
                  {
			if (level_check_move(x,y + 13) == 0)
                        {
			   y++;
			} else {
			   flying = false;
			}
		  }
         } else {
          if ((y < 80) && (y - y_velocity > 0)) {
		     if (level_check_move_full(x, y-y_velocity) == 0) {
		        y -= y_velocity;
		     } else {
			    y_velocity = 0;
			 }
		  }
         }  
	   }

       if (y_velocity > 0) y_velocity--; 
	   
     } else{
       breaking = false;
     }
  
     overlaybitmap(x, y, knight_bitmap + ( frame * SIZEOF_KNIGHT_RECORD));
  } else {
     count++;
  }
}

void knight_move(char new_facing)
{
  //if we are still moving in the same direction, accelerate
  if (new_facing == facing)
  {
    if (h_velocity <= MAX_VELOCITY) h_velocity++;
  } else {
    //test to see if the knight is at a full stop
    if (h_velocity < 1)
	{
	    facing = new_facing;
	    if (facing == FACING_LEFT)
	    {
	        frame = KNIGHT_MOVE_LEFT;
	    } else {
	        frame = KNIGHT_MOVE_RIGHT;
	    }
	    h_velocity = 1;

	//otherwise, were were in motion so act appropriately  
     } else {
       //if we were going fast enough, start breaking
	 breaking = true;
	 if (facing == FACING_LEFT)
	 {
	  frame = KNIGHT_BREAK_LEFT;
	 } else {
	  frame = KNIGHT_BREAK_RIGHT;
	 }
	 h_velocity--;
      }
  }
}

void knight_fly()
{
  flying = true;
  y_velocity = MAX_VELOCITY;
}

char knight_getState()
{
  return state;
}
