#include <avr/pgmspace.h>
#include <TVout.h>
#include <string.h>
#include "room.h"
#include "room_data.h"
#include "monster.h"

RoomElement roomElements[MAX_ELEMENT_RECORDS];
char element_count = 0;

//load room elements (if present)
void loadRoomElemments(char room)
{
  //clear the array of the previous contents
  memset(roomElements, 0, sizeof(roomElements));
  element_count = 0;
  
  int index_ptr = 0;
  
  //determine the index start for room elements
  index_ptr = pgm_read_byte_near(room_element_index_data + room);

  //read until we have reached the max number of element records
  if (index_ptr < 255)
  {
    //set to the actual data position
	index_ptr *= 4;
	while (element_count < MAX_ELEMENT_RECORDS)
    {
      roomElements[element_count].id = element_count;
      roomElements[element_count].type = pgm_read_byte_near(room_element_data + index_ptr);
      roomElements[element_count].x = pgm_read_byte_near(room_element_data + (index_ptr+1));
      roomElements[element_count].y = pgm_read_byte_near(room_element_data + (index_ptr+2));
      roomElements[element_count].step = 1;
      roomElements[element_count].state = STATE_VISIBLE;
      element_count++;
	
      //look to see if we have reached the end of data for the room
      if (pgm_read_byte_near(room_element_data + (index_ptr+3)) == 255) break;
	
      //otherwise, we continue advancing through the data
	  index_ptr += 4;
    }
  }
}

//handles the room elements
void handleRoomElements()
{
  for (char i=0; i < element_count; i++)
  {
    if (roomElements[i].state > STATE_HIDDEN)
	{
	   if (roomElements[i].type < 50) roomElements[i] = moveMonster(roomElements[i]);
	}
  }
}

//handles updating the room element
void updateRoomElement (RoomElement element)
{
  roomElements[element.id] = element;
}
