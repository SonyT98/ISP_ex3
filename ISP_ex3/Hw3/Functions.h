#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H___

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

// Function Declarations -------------------------------------------------------

/*
* GetHotel is reading the rooms file and updae the hotel struct.
* Input Argument:
*	our_hotel - pointer to hotel struct the includes the rooms information.
* Output Argument:
*	the function update the hotel struct. return -1 if there is an error.
*/
int GetHotel(struct hotel **our_hotel_pointer);

/*
* GetCostumer is reading the names file and updae the costumers struct.
* Input Argument:
*	our_costumers - pointer to array of costumers struct the includes the costumers information.
*	n_costumers   - number of costumers.
* Output Argument:
*	the function update the costumers struct. return -1 if there is an error.
*/
int GetCostumers(struct costumer ***our_costumers, int *n_costumers);


/*
* FreeArrayOfPointers free all the pointers in array of pointers, until the index
* i.
* Input Arguments:
*	arr   - the array of pointers (costumer) that we want to free.
*	index - how much allocated memory we want to free.
* Output Arguments: 
*	return -1 if failed.
*/
int FreeArrayOfPointers(struct costumer **arr, int index);

/*
* SemaphoreIntialize intialie all the semaphores and mutex to the wanted values
*/
#endif // __FUNCTIONS_H___