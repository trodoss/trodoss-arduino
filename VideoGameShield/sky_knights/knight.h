#ifndef __KNIGHT__
#define __KNIGHT__

#define KNIGHTSTATE_PLAYING 0
#define KNIGHTSTATE_DEAD 1

#define KNIGHT_MOVE_LEFT 0
#define KNIGHT_BREAK_LEFT 4
#define KNIGHT_FLY_LEFT 5

#define KNIGHT_MOVE_RIGHT 7
#define KNIGHT_BREAK_RIGHT 11
#define KNIGHT_FLY_RIGHT 12

#define FACING_LEFT 1
#define FACING_RIGHT 2

void knight_move(char new_facing);
void knight_fly();
void knight_draw();
char knight_getState();

#endif __KNIGHT__
