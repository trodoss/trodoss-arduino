#include <avr/pgmspace.h>
#include <TVout.h>
#include "knight.h"
#include "level.h"
#include "backdrop_bitmap.h"
#include "level_data.h"

#define SIZEOF_BACKDROP_RECORD 6

extern TVout TV;
char level_current = 0;

void level_update()
{
   knight_draw();
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
          if (current_block > 0) TV.bitmap(x * 8, y * 4, backdrop_bitmap + ((current_block-1) * SIZEOF_BACKDROP_RECORD));
      }
   }
}

//compare the given x,y coordinate against the level data
char level_check_move(char x, char y)
{
   char level_x = x / 8;
   char level_y = y / 4;
   if ((level_x > 15) || (level_y > 21)) return 0;
   return level_get_block(level_x, level_y);
}

//compare the given x,y coordinate against the level data (compares against an entire 8x12 block)
char level_check_move_full(char x, char y)
{
  char returnVal = 0;
  for (char test_y = y; test_y < (y + 13); test_y += 4)
  {
    returnVal = level_check_move(x, test_y);
    if (returnVal > 0) return returnVal;
  }
  return 0;
}

//return the block at a given 'level' x,y coordinate in the current level
char level_get_block(char level_x, char level_y)
{
   return pgm_read_byte_near (level_data + (level_current * LEVEL_SIZE) + level_x + (level_y * LEVEL_WIDTH));
}
