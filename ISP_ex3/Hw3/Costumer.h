#pragma once

#ifndef __COSTUMER_H__
#define __COSTUMER_H___

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

int day;
int count;
int num_people_entring_today;
int *rooms_waiting_list = NULL; //(N rooms)
int *out_days = NULL; //(N costumers)
HANDLE *checkout			= NULL; //(N costumers)
HANDLE god_signal			= NULL;
HANDLE first_day_barrier	= NULL;

int num_costumers = 0;
int num_rooms	  = 0;

// rendezvous barrier 
static HANDLE barrier_mutex		= NULL;
static HANDLE barrier_semaphore = NULL;
static int barrier_count		= 0;

// Mutex for critical reign  

static HANDLE file_mutex		= NULL;
static HANDLE a_mutex			= NULL;


/*
*	Costumer is the thread of each costumer that want to enter the 
*	the hotel. the thread try to enter the appropriate room and wait if cant enter in the first day.
*	when entered the costumer write to log file the input date, then the costumer stay in the hotel
*	until the day to leave will reach.

*/

DWORD Costumer(LPSTR lpparam);

/*
* int firstDayPreperation(int room_index, int room_size).
* This function update the variables num_people_entering_today and room_waiting_list
* before the first day starts, under mutex protection.
*
* Input Arguments :
*	room_index:		the index of the costumers room in the rooms array.
*	room_size:		int of the total size of the room.
* Return :
*	return -1 if error accord
*/

int firstDayPreperation(int room_index, int room_size);

/*
* int preFirstDayBarrier()
* This function makes sure that all threads start first day together using
* rendezvous barrier.
*
* Return :
*	return -1 if error accord
*/
int preFirstDayBarrier();


/*
* int tryToEnterTheRoom(int room_index, HANDLE room_semaphore).
* This function "try to enter the room" by doing wait on the room semaphore
*
* Input Arguments :
*	room_index:		the index of the costumers room in the rooms array.
*	room_semaphore:		Handle to the room semaphore.
* Return :
*	return -1 if error accord
*/


int tryToEnterTheRoom(int room_index, HANDLE room_semaphore);

#endif // __THREADFUNCTIONS_H___