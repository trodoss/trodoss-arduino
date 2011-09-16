#include <TVout.h>
#include "sound_data.h"

#define SONG_PLAYING 0
#define SONG_ENDED 1

extern TVout TV;

int song_note_len;       //current song's not length
int current_note;        //current note
int song_end_pos;        //indicates the position of the last note of the song
char music_state;        //indicates the current music state

//handles the music/sfx playing for the sketch
void update_sound()
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
      int note_duration = ( song_note_len / pgm_read_byte_near(duration + current_note));
  	  
      next_note_start_time = TV.millis()+note_duration;
	  
	  //play the note
      TV.tone(pgm_read_word_near(melody + current_note), note_duration); 
  	
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

