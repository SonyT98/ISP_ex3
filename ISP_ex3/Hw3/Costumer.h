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

/*
*	Costumer is the thread of each costumer that want to enter the 
*	the hotel. the thread try to enter the appropriate room and wait if cant enter in the first day.
*	when entered the costumer write to log file the input date, then the costumer stay in the hotel
*	until the day to leave will reach.

*/

DWORD Costumer_thread(LPSTR lpparam);

/*
* This function update the variables num_people_entering_today and room_waiting_list
* before the first day starts, under mutex protection.
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/


int firstDayPreperation(struct costumer* costumer, struct hotel* hotel);

/*
* int preFirstDayBarrier(int num_costumers)
* This function makes sure that all threads start first day together using
* rendezvous barrier.
* Input Arguments :
*	num_costumers:	the total number of costumers\threads.
* Return :
*	return -1 if error accord
*/
int preFirstDayBarrier(int num_costumers);


/*
* This function "try to enter the room" by doing wait on the room semaphore
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/
int tryToEnterTheRoom(costumer* costumer,hotel* hotel);



/*
* This function write that the costumer entered the room.
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
*	in_out:			if equal IN the input line will be written
					if equal OUT the output line will be written
* Return :
*	return -1 if error accord
*/
int writeToFile(costumer* costumer,hotel* hotel, int in_out);


/*
* This function calculate the day this costumer will exit the hotel and fill the exit day
* in the out_days array
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/
int fillOutDay(costumer* costumer,hotel* hotel);

/*
* This function add 1 to the count and check if the number of people that entered today
* is equal to the expected number of people to enter. if it does it Signals God thread to change the day.
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/
int checkEndOfDay(costumer* costumer,hotel* hotel);


/*
* This function waits until the God thread release it when the day to exit will come.
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/
int accommodateRoom(costumer* costumer,hotel* hotel);

/*
* This function releases the room semaphore by 1
*
* Input Arguments :
*	costumer:		pointer to the costumer structure.
*	hotel:			pointer to the global hotel structure.
* Return :
*	return -1 if error accord
*/
int freeRoom(costumer* costumer, hotel* hotel);

#endif // __THREADFUNCTIONS_H___