#include <avr/pgmspace.h>
#include <TVout.h>
#include "poofy.h"
#include "poofy_bitmap.h"
#include "bitmap_funcs.h"
#include "level.h"

extern TVout TV;

#define MAX_VELOCITY 4
#define STEP_LENGTH 2
#define SIZEOF_POOFY_RECORD 10

char x_velocity = 0;
char y_velocity = 0;
char x = 8;
char y = 8;
char facing = FACING_LEFT;
char frame = 0;
char count = 0;
char state = POOFY_STANDING;

void poofy_draw()
{
     char tile_adjust;
     
     //handle delay for changing x velocity, y velocity, or when jumping (special case)
     if ((count >= MAX_VELOCITY - x_velocity) || (y_velocity > 0) || (state == POOFY_JUMPING))
     {
         //erasebitmap(x, y, poofy_bitmap);
         eraseBitmapRect(x, y, poofy_bitmap);
         frame++; 
         count = 0;
     
	 //handle x axis
         if (x_velocity > 0)
         {
            if (facing == FACING_RIGHT)
	    {
             //if we have hit someting moving right, adjust accordingly
		if (level_check_move_v(x + x_velocity + 8, y, tile_adjust) > 0)
                {
                   x = (tile_adjust * 8) - 9;
                   x_velocity = 0;
                //otherwise, keep moving right 
                } else {		
		   x += x_velocity;
                   x_velocity--;
                }		
	   } else {
               if (level_check_move_v(x - x_velocity, y, tile_adjust) > 0)
               {
                   x = ((tile_adjust + 1) * 8) + 1;
                   x_velocity = 0;
               } else {
		   x -= x_velocity;
                   x_velocity--;
               }	    
	    }
         }
	 
	 //handle y axis
	 if (y_velocity > 0)
	 {
	    //moving up
	    if (state == POOFY_JUMPING)
	    {
		   if (level_check_move_h(x, y-y_velocity, tile_adjust) > 0)
		   {
		      y = ((tile_adjust + 1) * 8) + 1;
		      y_velocity = 1;
	              //poofy starting to freefall
		     state = POOFY_FALLING;
		   } else {
		      y -= y_velocity;
		      y_velocity--;
		   }
	    } else {
		   if (level_check_move_h(x, y+y_velocity+8, tile_adjust) > 0)
		   {
   		      y = (tile_adjust * 8) - 9;
		      y_velocity = 0;
		      //poofy standing on ground
		      state = POOFY_STANDING;
		   } else {
		      y += y_velocity;
			  if (y_velocity <= MAX_VELOCITY) y_velocity++;
		   }
	   } 
	 } else {
	    //test for gravity pulling down (if nothing is below)
		if (level_check_move_h(x, y+9, tile_adjust) == 0)
		{
		   state = POOFY_FALLING;
		   y_velocity = 1;
		}
	 }
	 
	 //determine animation frames to show
	 if (y_velocity > 0)
	 {
            if (facing == FACING_LEFT) frame = POOFY_JUMP_LEFT;
            if (facing == FACING_RIGHT) frame = POOFY_JUMP_RIGHT;
	 } else {
	    if (x_velocity > 0)
            {
	       if ((facing == FACING_LEFT) && (frame > 6)) frame = POOFY_MOVE_LEFT;
               if ((facing == FACING_RIGHT) && (frame > 2)) frame = POOFY_MOVE_RIGHT;
            } else {
	        if (facing == FACING_LEFT) frame = POOFY_FACE_LEFT;
                if (facing == FACING_RIGHT) frame = POOFY_FACE_RIGHT;
            }
         }
	 
        overlaybitmap(x, y, poofy_bitmap + ( frame * SIZEOF_POOFY_RECORD));
     } else {
       count++;
     }
}

void poofy_move(char new_facing)
{
  if (state < POOFY_FALLING)
  {
     //if we are still moving in the same direction, accelerate
     if (new_facing == facing)
     {
       if (x_velocity <= MAX_VELOCITY) x_velocity++;
     } else {
       facing = new_facing;
       if (facing == FACING_LEFT)
       {
          frame = POOFY_MOVE_LEFT;
       } else {
          frame = POOFY_MOVE_RIGHT;
       }
       x_velocity = 1;
     }
   }
}

void poofy_jump()
{
  //make sure that we are in a state where jumping is possible
  //(can't jump if already jumping/falling)
  //if (y_velocity == 0)
  if (state < POOFY_JUMPING)
  {
    y_velocity = MAX_VELOCITY;
    state = POOFY_JUMPING;
  }  
}

char poofy_getState()
{
  return state;
}
