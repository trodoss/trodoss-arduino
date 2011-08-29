/*
   A ringtone (music) player for the Wayne and Layne Video Game Shield.
   
   2011 - trodoss
   http://www.savagecircuits.com
   
   The music used in this project was converted from RTTTL (Ring Tone Text Transfer Language)
   that was available at http://merwin.bespin.org/db/rts/ (MeRWiN's Ringtone Search)
   
   
      
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
 
#include <i2cmaster.h>
#include <nunchuck.h>
#include <TVout.h>
#include <font6x8.h>
#include "tones.h"

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define PAUSE_BETWEEN_ACTIONS 100

#define SONG_PLAYING 0
#define SONG_ENDED 1

TVout TV;
Nunchuck player1;

int song_note_len;       //current song's note length
int current_note;        //current note
int song_end_pos;        //indicates the position of the last note of the song
char music_state;        //indicates the current music state
char buffer[16];         //buffer for display data

char selected;           //indicates the selected menu option
char page_start;         //indicates which item the page starts on

void setup()
{
  TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);
  TV.select_font(font6x8);
  player1.begin(0);
  
  selected = 0;
  page_start = 0;
  draw_song_menu(0);
}

void loop()
{
  handle_song_menu();
  update_music();
}

void draw_song_menu(char start)
{
  //clear the contents of the screen
  TV.clear_screen();
  
  //draw out the song titles on the screen
  char line = 1;
  for (char i = start; i < (start+8); i++)
  {
    strcpy_P(buffer, (char*)pgm_read_word(&title_table[i]));
    TV.print(1, line, buffer);
	line +=9;
  }
  
  //draw select box around the current item
  TV.draw_rect(0, 0, 98, 8, 1);
  selected = 0;

  //draw bottom portion of the screen
  TV.draw_line(0, 76, 128, 76, 1);
  TV.printPGM(1, 80, PSTR("L=Back/R=Next"));
}

void handle_song_menu()
{
      static long next_action = 0;
      if (TV.millis() >= next_action)
      {      
        player1.update();
       
        if (player1.joy_up())
        {
  	  if (selected > 0)
  	  {
            TV.draw_rect(0, selected * 9, 98, 8, 0);
  	    selected--;
  	    TV.draw_rect(0, selected * 9, 98, 8, 1);
  	  }
        }
  	  
        if (player1.joy_down())
        {
          if (selected < 7)
  	  {
            TV.draw_rect(0, selected * 9, 98, 8, 0);
  	    selected++;
  	    TV.draw_rect(0, selected * 9, 98, 8, 1);	  
  	  }
        }
        
        if (player1.joy_right())
        {
         if (page_start < (TOTAL_SONGS -8))
         {
           page_start += 8;
           draw_song_menu(page_start);
         }
        }

        if (player1.joy_left())
        {
         if (page_start > 0)
         {
           page_start -= 8;
           draw_song_menu(page_start);
         }
        }
                  
        if (player1.button_z())
        {
          play_song(selected + page_start);
        }	
      
        next_action = TV.millis()+PAUSE_BETWEEN_ACTIONS;  
      }
}

//handles the music playing for the sketch
void update_music()
{
  if (music_state == SONG_PLAYING)
  {
    static long next_note_start_time = 0;
    if (TV.millis() >= next_note_start_time)
    {
      if (current_note == song_end_pos)
      {
         //it's the end of the song
         music_state = SONG_ENDED;  
      }
      
	  //determine the current note duration
      int note_duration = ( song_note_len / pgm_read_byte_near(duration_t1_c1 + current_note));
  	  
      next_note_start_time = TV.millis()+note_duration;
	  
	  //play the note
      TV.tone(pgm_read_word_near(melody_t1_c1 + current_note), note_duration); 
  	
      current_note++;  
    }
  }
}

//starts the playing of the selected song
void play_song(char song)
{
  //determine the note length based on the tempo of the song
  song_note_len = ( 60000 / pgm_read_word_near(tempo + song)) * 4;
  
  //determine the end position of the song
  song_end_pos = pgm_read_word_near(song_start + (song+1)) - 1;
  
  //set the note to the first note of the song
  current_note = pgm_read_word_near(song_start + song);
  
  //set the state to 'playing'
  music_state = SONG_PLAYING;
}

