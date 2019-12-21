#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H___

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

#include "HardCodedData.h"

/*oOoOoOoOoOoOoOoOoOoO Global Variables oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

extern int day;
extern int count;
extern int num_people_entring_today;
extern int rooms_waiting_list[MAX_NUM_ROOMS]; //(N rooms)
extern int out_days[MAX_NUM_COSTUMERS]; //(N costumers)

//semaphores
extern HANDLE checkout[MAX_NUM_COSTUMERS]; //(N costumers)
extern HANDLE god_signal;


// rendezvous barrier 
extern HANDLE barrier_mutex;
extern HANDLE barrier_semaphore;
extern int barrier_count;

// Mutex for critical reign  
extern HANDLE file_mutex;
extern HANDLE a_mutex;
extern HANDLE count_mutex;

#endif
