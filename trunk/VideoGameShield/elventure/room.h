#ifndef __ROOM__
#define __ROOM__

#define SIZEOF_ELEMENT_RECORD 4
#define MAX_ELEMENT_RECORDS 4

#define MONSTER_OCTO 0
#define MONSTER_GOBO 1

#define ITEM_SWORD 50

#define STATE_HIDDEN     0
#define STATE_VISIBLE    1
#define STATE_MOVE_UP    2
#define STATE_MOVE_DOWN  3
#define STATE_MOVE_LEFT  4
#define STATE_MOVE_RIGHT 5

struct RoomElement
{
  unsigned char id;
  unsigned char type;
  unsigned char x;
  unsigned char y;
  unsigned char state;  
  unsigned char step;
};

void loadRoomElemments(char room);
void handleRoomElements();
void updateRoomElement (RoomElement element);
RoomElement getRoomElement(char id);
bool testRoomElement (RoomElement element, char testX, char testY, char ySize);

#endif __ROOM__
