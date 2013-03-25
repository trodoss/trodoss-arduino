#ifndef __SOUND__
#define __SOUND__

#include <avr/pgmspace.h>

void play_song(char song);
void play_song_once(char song);
void update_sound();
void play_sfx(char song);

#endif __SOUND__
