#pragma once
#ifndef __HARDCODEDDATA_H__
#define __HARDCODEDDATA_H___

#define ERROR_CODE ((int)(-1))

typedef struct
{
	char *name;
	int money;
	int index;
	int my_room;
} costumer;

typedef struct
{
	char **room_names;
	HANDLE *rooms_sem;
	int *rooms_size;
	int *price_per_person;
	int number_of_rooms;
} hotel;

typedef struct
{
	hotel *hotel;
	costumer *costumer;
	int N_costumers;
} Costumer_arg;

#endif // __HARDCODEDDATA_H___