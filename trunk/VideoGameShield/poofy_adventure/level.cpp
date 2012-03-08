#include <avr/pgmspace.h>
#include <TVout.h>
#include "poofy.h"
#include "level.h"
#include "level_bitmap.h"
#include "level_data.h"

#define SIZEOF_BACKDROP_RECORD 10

extern TVout TV;
char level_current = 0;

void level_update()
{
   poofy_draw();
}

void level_advance()
{
  level_current++;
  level_draw();
}

void level_draw()
{
   TV.clear_screen();
   
   //read the level data and draw the level on the screen
   for (char y=0; y < LEVEL_HEIGHT; y++)
   {
      for (char x=0; x < LEVEL_WIDTH; x++)
      {
          char current_block = level_get_block(x, y);
          if (current_block > 0) TV.bitmap(x * 8, y * 8, level_bitmap + ((current_block-1) * SIZEOF_BACKDROP_RECORD));
      }
   }
}

char level_check_move_h(char x, char y, char &tile_y)
{
   char tile_x_pixels = (x - (x % 8)); 	//calculate the x position in pixels we are checking against
   char testend = x + 8; //calculate the end of testing
   tile_y = y >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   char tile_x = tile_x_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_x_pixels <= testend){
       if(level_get_block(tile_x, tile_y) > 0)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_x++;		//increase tile x map coordinate
       tile_x_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

char level_check_move_v(char x, char y, char &tile_x)
{
   char tile_y_pixels = (y - (y % 8)); 	//calculate the x position in pixels we are checking against
   char testend = y + 8; //calculate the end of testing
   tile_x = x >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   char tile_y = tile_y_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_y_pixels <= testend){
       if(level_get_block(tile_x, tile_y) > 0)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_y++;		//increase tile x map coordinate
       tile_y_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

//return the block at a given 'level' x,y coordinate in the current level
char level_get_block(char level_x, char level_y)
{
   return pgm_read_byte_near (level_data + (level_current * LEVEL_SIZE) + level_x + (level_y * LEVEL_WIDTH));
}
