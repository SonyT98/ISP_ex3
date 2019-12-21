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
	costumer *me = NULL;
	hotel *my_hotel = NULL;
	int n = 0, err = 0, wait_res = 0;

	err = GetHotel(&my_hotel);
	err = GetCostumers(&costumers,&n);
	err = SemaphoreIntialize(my_hotel, n);
	return 0;
}