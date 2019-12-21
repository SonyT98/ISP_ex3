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


int main()
{
	costumer **costumers = NULL;
	hotel *my_hotel = NULL;
	Costumer_arg **c_arg = NULL;
	God_arg *g_arg = NULL;
	char *c = NULL;

	int n = 0, err = 0, wait_res = 0;

	err = GetHotel(&my_hotel);
	err = GetCostumers(&costumers,&n);
	err = SemaphoreIntialize(my_hotel, n);
	err = CreateCostumersAndGodArg(my_hotel, costumers, n, &c_arg, &g_arg);
	err = FindMyRoom(my_hotel, costumers, n);
	return 0;
}