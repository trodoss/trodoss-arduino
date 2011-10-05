#ifndef __ELF__
#define __ELF__
#include "room.h"

#define FACING_DOWN  0
#define FACING_UP    3
#define FACING_LEFT  6
#define FACING_RIGHT 9

#define STEP_LENGTH  4

struct Elf
{
  unsigned char facing;
  unsigned char step;
  unsigned char x;
  unsigned char y;
  unsigned char hearts;
  unsigned char crystals;
};

void showElf();
void moveElf(unsigned char facing);
void throwSword();
RoomElement hitElf(RoomElement element);
Elf getElf();

#endif __ELF__
