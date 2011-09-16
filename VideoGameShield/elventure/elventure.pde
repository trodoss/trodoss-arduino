/*                                   __    
                                   /___/
                                   / /
   /-------------/              __/_/__
     |  |-------/       //===//__ *__ //===//
     |  .----/                 / / /
     |  .---/                 / / /
     |  |------/ 
   /----------/ L + V + E + N + T + U + R + E *=-
   An Adventure Game for the Arduino/Video Game Shield 
 
   2011 - trodoss
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.   
   
 */
 
#include <stdlib.h> 
#include <i2cmaster.h>
#include <nunchuck.h>
#include <TVout.h>
#include <font4x6.h>

#include "map.h"
#include "room.h"
#include "elf.h"
#include "sound.h"

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define PAUSE_BETWEEN_ACTIONS 100

TVout TV;
Nunchuck player1;

void setup()
{
  TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);
  TV.select_font(font4x6);
  
  randomSeed(analogRead(0));
  
  player1.begin(0);
  
  drawMapRoom();
  
  showElf();
  
  play_song(0);
}

void loop()
{  
  static long next_action = 0;
  
  if (TV.millis() >= next_action)
  {      
     player1.update();
       
    if (player1.joy_up()) moveElf(FACING_UP);
    if (player1.joy_down()) moveElf(FACING_DOWN);
    if (player1.joy_right()) moveElf(FACING_RIGHT);
    if (player1.joy_left()) moveElf(FACING_LEFT);
    
    next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
	
    handleRoomElements();
  }
  update_sound();
}
