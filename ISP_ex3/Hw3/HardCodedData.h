#pragma once
#ifndef __HARDCODEDDATA_H__
#define __HARDCODEDDATA_H___

#define ERROR_CODE ((int)(-1))
#define MAX_LINE_LENGTH 50
#define MAX_NUM_ROOMS 5
#define MAX_ROOM_NAME 20
#define MAX_NUM_COSTUMERS 15

#define ENTER 5
#define EXIT 7


typedef struct
{
	char name[MAX_LINE_LENGTH];
	int money;
	int index;
	int my_room;
} costumer;

typedef struct
{
	char room_names[MAX_NUM_ROOMS][MAX_ROOM_NAME];
	HANDLE rooms_sem[MAX_NUM_ROOMS];
	int rooms_size[MAX_NUM_ROOMS];
	int price_per_person[MAX_NUM_ROOMS];
	int number_of_rooms;
} hotel;

typedef struct
{
	hotel *hotel;
	costumer *costumer;
	int N_costumers;
} Costumer_arg;

typedef struct
{
	costumer **costumers;
	int N_costumers;
} God_arg;


#endif // __HARDCODEDDATA_H___