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
* GetHotel is reading the rooms file and update the hotel struct.
* Input Argument:
*	our_hotel - pointer to hotel struct the includes the rooms information.
* Output:
*	the function update the hotel struct. return -1 if there is an error.
*/
int GetHotel(struct hotel **our_hotel_pointer);

/*
* GetCostumer is reading the names file and update the costumers struct.
* Input Argument:
*	our_costumers - pointer to array of costumers struct the includes the costumers information.
*	n_costumers   - number of costumers.
* Output:
*	the function update the costumers struct. return -1 if there is an error.
*/
int GetCostumers(struct costumer ***our_costumers, int *n_costumers);


/*
* FreeArrayOfPointers free all the pointers in array of pointers, until the index
* i.
* Input Arguments:
*	arr   - the array of pointers (costumer) that we want to free.
*	index - how much allocated memory we want to free.
*/
void FreeArrayOfPointers(struct costumer **arr, int index);


/*
* SemaphoreIntialize initialize all the semaphores and mutex to the wanted values.
* Input Arguments:
*	our_hotel			- initialize the semaphores of the rooms.
*	number_of_costumers - number of costumers.
* Output:
*	return -1 if failed;
*/
int SemaphoreIntialize(struct hotel *our_hotel, int number_of_costumers);


/*
* CreateCostumersAndGodArg set the god and costumers arguments (to the threads)
* to the wanted values.
* Input Arguments:
*	our_hotel - the hotel structure we created.
*	our_costumers - the array of costumers structure we created.
*	n_costumers - number of costumers.
*	c_arg - costumer argument.
*	g_arg - god argument.
* Output:
*	return -1 if there is an error.
*/
int CreateCostumersAndGodArg(struct hotel *our_hotel,struct costumer **our_costumers, int n_costumers,
	struct Costumer_arg ***c_arg,struct God_arg **g_arg);


/*
* FindMyRoom set the room for every costumer according to their money.
* Input Arguments:
*	our_hotel - the hotel structure we created.
*	our_costumers - the array of costumers structure we created.
*	n_costumers - number of costumers.
* Output:
*	return -1 if there is an error.
*/
int FindMyRoom(struct hotel *our_hotel, struct costumer **our_costumers, int n_costumers);

/*
* FreeHotel free the pointer to the hotel struct.
* InputArguments:
*	our_hotel - hotel struct that we want to free.
*/
void FreeHotel(struct hotel *our_hotel);

/*
* FreeHotel free the pointer to the costumers struct array and the costumers struct.
* InputArguments:
*	our_costumers - costumers array struct that we want to free.
*	n_costumers   - number of costumers.
*/
void FreeCostumers(struct costumer **our_costumers, int n_costumers);

/*
* FreeMutexAndSemaphoresHandles free all the handles to the semaphores and mutexs
* Input ArgumentsL
*	our_hotel - the pointer to the hotel struct that includes the semaphores.
*	n_costumers   - number of costumers.
*/
void FreeMutexAndSemaphoresHandles(struct hotel* our_hotel, int n_costumers);

/*
* FreeCostumersAndGodArg free the allocated memory for the thread arguments.
* Input Arguments:
*	c_arg - the costumer threads arguments.
*	g_arg - the god thread argument.
*/
void FreeCostumersAndGodArg(struct Costumer_arg **c_arg,struct God_arg *g_arg);


/*
* runHotel is the main function that operates the hotel program.
* the function initialize all memory and structure needed for the hotel,
* runs the costumers threads and free all needed memory and handles.
*/

int runHotel();

#endif // __FUNCTIONS_H___