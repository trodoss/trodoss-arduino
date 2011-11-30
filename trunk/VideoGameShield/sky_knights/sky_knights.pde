/*

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
   
 */
 
#include <stdlib.h> 
#include <i2cmaster.h>
#include <nunchuck.h>
#include <TVout.h>
#include <font4x6.h>

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define PAUSE_BETWEEN_ACTIONS 50

#define GAME_TITLE   0
#define GAME_PLAYING 1
#define GAME_PAUSED  2
#define GAME_WON     3
#define GAME_OVER    4

#include "sound.h"
#include "logo.h"
#include "level.h"
#include "knight.h"

TVout TV;
Nunchuck player1;
char game_state;

void setup()
{
  TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);
  TV.select_font(font4x6);
  
  randomSeed(analogRead(0));
  
  player1.begin(0);
  start_title();
}

void loop()
{  
  static long next_action = 0;
  switch (game_state)
  {
    case GAME_TITLE:
	case GAME_OVER:
	case GAME_WON:
       if (TV.millis() >= next_action)
       {
         player1.update();
		 //wait for a button to be pressed to continue
         if ((player1.button_c()) || (player1.button_z()))
         {
		   
		   //check to see what we are doing when we start the game
		   if (game_state > GAME_TITLE)
		   {
		     if (game_state == GAME_WON)
			 {
				//to do: win game
			 } else {
				//to do: handle returning to the title
			 }
		   }
		   
		   game_state = GAME_PLAYING;
		   start_game();
         }
		         
         next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
       }	
	   sound_update();
	   break;
	   
	case GAME_PLAYING:
       if (TV.millis() >= next_action)
       {      
         player1.update();
       
         if (player1.joy_right()) knight_move(FACING_RIGHT);
         if (player1.joy_left()) knight_move(FACING_LEFT);
         if (player1.button_z()) knight_fly();
         if (player1.button_c()) game_state = GAME_PAUSED;
    
         next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
	
         level_update();
		 
		 //check the knight's state
		 if (knight_getState() != KNIGHTSTATE_PLAYING)
		 {
		   if (knight_getState() == KNIGHTSTATE_DEAD)
		   {
		     start_game_over();
		   } else {
		     start_game_won();
		   }
		 }
       }
       sound_update();
       break;
       
   case GAME_PAUSED:
       if (TV.millis() >= next_action)
       {
         player1.update();
		 
         if (player1.button_c()) game_state = GAME_PLAYING;
         
         next_action = TV.millis() + PAUSE_BETWEEN_ACTIONS; 
       }
       break;
  }
}

void start_game()
{
  game_state = GAME_PLAYING;  
  level_draw();
}

void start_title()
{
  game_state = GAME_TITLE;
  logo_draw();
}

void start_game_over()
{
}

void start_game_won()
{
}
