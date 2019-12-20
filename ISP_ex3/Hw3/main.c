/*oOoOoOoOoOoOoOoOoOoO Libary Includes oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

/*oOoOoOoOoOoOoOoOoOoO Project Includes oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "Functions.h"
#include "HardCodedData.h"
#include "ThreadFunctions.h"

/*oOoOoOoOoOoOoOoOoOoO Global Variables oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int day;
int count;
int num_people_entring_today;
int *rooms_waiting_list = NULL; //(N rooms)
int *out_days = NULL; //(N costumers)
HANDLE *checkout = NULL; //(N costumers)
HANDLE god_signal = NULL;
HANDLE first_day_barrier = NULL;

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int main()
{

}