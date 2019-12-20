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
	char **romm_names;
	HANDLE *romms_sem;
	int *price_per_person;
	int number_of_rooms;
} hotel;

typedef struct
{
	hotel *hotel;
	costumer *costumer;
	int N_costumers;
} thread_arg;

#endif // __HARDCODEDDATA_H___