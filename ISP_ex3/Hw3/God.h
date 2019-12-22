#pragma once
#ifndef __GOD_H__
#define __GOD_H___


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
* This thread waits until  all the costumers that needed to enter in a day
* enters the room. when it happens calculation of the next day is preform and,
* releasing all the thread that need to exit in that day.
*/
DWORD God_thread(LPSTR lpParam);


/*
* This function go over the out_days array and find the minimum out day.
* 
* Input Arguments :
*	num_costumers:	the Total number of costumers/ the size of outdays.
* Return :
*	return the next day 
*/

int findNextDay(int num_costumers);

/*
* CopyintArray copy array src into array dest. a and b are allocated and new to be the
* same size.
* Input Arguments:
*	src - the source array.
*	dest - the destination array.
*	n - the array's size.
*/
void CopyintArray(int *src, int *dest, int n);
#endif
