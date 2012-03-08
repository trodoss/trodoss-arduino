#ifndef __POOFY__
#define __POOFY__

//state constants
#define POOFY_STANDING 0
#define POOFY_WALKING 1
#define POOFY_JUMPING 2
#define POOFY_FALLING 3
//special constant - indicate that Poofy won the game
#define POOFY_WON 5
//special constants - handle Poofy dying animation and
//indicating that the game is over
#define POOFY_DYING 8
#define POOFY_DEAD 9

//animation constants
#define POOFY_FACE_RIGHT 0
#define POOFY_MOVE_RIGHT 1
#define POOFY_JUMP_RIGHT 3
#define POOFY_FACE_LEFT 4
#define POOFY_MOVE_LEFT 5
#define POOFY_JUMP_LEFT 7
#define POOFY_DYING 8

//facing constants
#define FACING_LEFT 1
#define FACING_RIGHT 2

void poofy_move(char new_facing);
void poofy_jump();
void poofy_draw();
char poofy_getState();

#endif __POOFY__
