#ifndef __LEVEL__
#define __LEVEL__

void level_draw();
void level_advance();
void level_update();
char level_check_move_h(char x, char y, char &tile_x);
char level_check_move_v(char x, char y, char &tile_x);
char level_get_block(char level_x, char level_y);

#endif __LEVEL__
