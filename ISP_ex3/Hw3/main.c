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
#include "Costumer.h"
#include "God.h"
#include "GlobalVariables.h"


int main(int argc, char** argv)
{

	costumer **costumers = NULL;
	hotel *my_hotel = NULL;
	Costumer_arg **c_arg = NULL;
	God_arg *g_arg = NULL;
	char *c = NULL;


	int n = 0, err = 0, wait_res = 0, j = 0;
	err = chdir(argv[1]);
	err = GetHotel(&my_hotel);
	err = GetCostumers(&costumers, &n);
	err = SemaphoreIntialize(my_hotel, n);
	err = FindMyRoom(my_hotel, costumers, n);
	err = CreateCostumersAndGodArg(my_hotel, costumers, n, &c_arg, &g_arg);
	err = CreateThreads(c_arg, g_arg);

	free(my_hotel);
	FreeArrayOfPointers(costumers, n);
	free(costumers);
	for (j = 0; j < my_hotel->number_of_rooms; j++)
		CloseHandle(my_hotel->rooms_sem[j]);
	CloseHandle(god_signal);
	CloseHandle(barrier_semaphore);
	for (j = 0; j < n; j++)
		CloseHandle(checkout[j]);
	CloseHandle(barrier_mutex);
	CloseHandle(file_mutex);
	CloseHandle(a_mutex);
	CloseHandle(count_mutex);

	return err;
}