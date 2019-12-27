// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 3 in the Course Introduction to system programming
// This is the main function of the moed bet inn


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

/*oOoOoOoOoOoOoOoOoOoO Main Function oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int main(int argc, char** argv)
{
	//variables
	costumer **costumers = NULL;
	hotel *my_hotel = NULL;
	Costumer_arg **c_arg = NULL;
	God_arg *g_arg = NULL;
	char *path = NULL;
	int n = 0, err = 0, wait_res = 0, j = 0;

	// check number of input arguments
	if (argc != 2)
	{
		if (argc > 2) printf("Error: Too many arguments for Hw3\n");
		else printf("Error: Too few arguments for Hw3\n");
		return ERROR_CODE;
	}

	//change the dir folder to the required folder
	path = argv[1];
	err = chdir(argv[1]);
	if (err == ERROR_CODE)
	{
		printf("Error: folder path isn't valid!\n");
		return ERROR_CODE;
	}

	//fill up hotel structure according to the rooms.txt file
	err = GetHotel(&my_hotel);
	if (err == ERROR_CODE) return ERROR_CODE;

	//fill up costumer structure array according to the names.txt file
	err = GetCostumers(&costumers, &n);
	if (err == ERROR_CODE) return ERROR_CODE;

	//Intialize the rooms semaphores and the global semaphores/mutes to the required values
	err = SemaphoreIntialize(my_hotel, n);
	if (err == ERROR_CODE) return ERROR_CODE;

	//for each costumer find his room
	err = FindMyRoom(my_hotel, costumers, n);
	if (err == ERROR_CODE) return ERROR_CODE;

	// create the arguments for costumer thread and god thread
	err = CreateCostumersAndGodArg(my_hotel, costumers, n, &c_arg, &g_arg);
	if (err == ERROR_CODE) return ERROR_CODE;

	//create the thread that write to roomLog
	err = CreateThreads(c_arg, g_arg);
	if (err == ERROR_CODE) return ERROR_CODE;

	//free memory and handles for the semaphore/mutex
	FreeMemoryAndHandles(c_arg, g_arg);

	return err;
}