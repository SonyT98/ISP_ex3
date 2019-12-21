#include "GlobalVariables.h"

/*oOoOoOoOoOoOoOoOoOoO Global Variables oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int day = 1;
int count = 0;
int num_people_entring_today = 0;
int rooms_waiting_list[MAX_NUM_ROOMS]; //(N rooms)
int out_days[MAX_NUM_COSTUMERS]; //(N costumers)

//semaphores
HANDLE checkout[MAX_NUM_COSTUMERS]; //(N costumers)
HANDLE god_signal = NULL;


// rendezvous barrier 
HANDLE barrier_mutex = NULL;
HANDLE barrier_semaphore = NULL;
int barrier_count = 0;

// Mutex for critical reign  
HANDLE file_mutex = NULL;
HANDLE a_mutex = NULL;
HANDLE count_mutex = NULL;

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/