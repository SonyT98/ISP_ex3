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
*	our_hotel - hotel struct the includes the rooms information.
* Output Argument:
*	the function update the hotel struct. return -1 if there is an error.
*/
int GetHotel(hotel *our_hotel);

/*
* GetCostumer is reading the names file and updae the costumers struct.
* Input Argument:
*	our_costumers - array of costumers struct the includes the costumers information.
* Output Argument:
*	the function update the costumers struct. return -1 if there is an error.
*/
int GetCostumers(costumer **our_costumers);


/*
* FreeArrayOfPointers free all the pointers in array of pointers, until the index
* i.
* Input Arguments:
*	arr   - the array of pointers (char) that we want to free.
*	index - how much allocated memory we want to free.
* Output Arguments: 
*	return -1 if failed.
*/
int FreeArrayOfPointers(char **arr, int index);


#endif // __FUNCTIONS_H___