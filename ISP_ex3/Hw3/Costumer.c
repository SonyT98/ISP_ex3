// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 3 in the Course Introduction to system programming
// This is the costumer thread function (and his related functions)

// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Costumer.h"
#include "GlobalVariables.h"

DWORD Costumer_thread(LPSTR lpParam)
{
	Costumer_arg *parameters = NULL;
	costumer* my_costumer;
	hotel* my_hotel;

	int num_costumers;
	HANDLE my_room_sem = NULL;
	DWORD wait_code;
	int ret_val;


	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return ERROR_CODE;
	}

	/* Convert (void *) to parameters type */
	parameters = (Costumer_arg *)lpParam;
	
	/* Convert the structure variable into local variables */
	my_costumer = parameters->costumer;
	my_hotel	 = parameters->hotel;
	num_costumers = parameters->N_costumers;

	ret_val = firstDayPreperation(my_costumer,my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	/* barrier before first day starts */
	ret_val = preFirstDayBarrier(num_costumers+1);
	if (ret_val == ERROR_CODE) return ERROR_CODE;
	
	/* ---ALL THREADS STARTS TOGETHER HERE--- */

	/* try to enter the room if room is full wait until someone gets out */
	ret_val = tryToEnterTheRoom(my_costumer,my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	/* -- Entered the room --*/
	ret_val = writeToFile(my_costumer, my_hotel, ENTER);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	//fill the out days array
	ret_val = fillOutDay(my_costumer, my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = checkEndOfDay(my_costumer, my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = accommodateRoom(my_costumer, my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = writeToFile(my_costumer, my_hotel, EXIT);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = freeRoom(my_costumer, my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = checkEndOfDay(my_costumer, my_hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	return 0;
}

int firstDayPreperation(costumer* costumer,hotel* hotel)
{

	DWORD wait_code;
	BOOL ret_val;
	int room_index = costumer->my_room;
	int room_size = hotel->rooms_size[room_index];


	wait_code = WaitForSingleObject(a_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for a_mutex\n");
		return ERROR_CODE;
	}
	/*** critical Regan ***/

	// add me to the waiting list
	rooms_waiting_list[room_index]++;

	if (rooms_waiting_list[room_index] <= room_size)
	{
		num_people_entring_today++;
	}

	ret_val = ReleaseMutex(a_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing a_mutex\n");
		return ERROR_CODE;
	}

	return 0;
}

int preFirstDayBarrier(int num_costumers)
{
	DWORD wait_code;
	BOOL ret_val;
	int ret = 0;
	
	// down(mutex)
	wait_code = WaitForSingleObject(barrier_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for barrier_mutex\n");
		return ERROR_CODE;
	}
	/* critical section */
	barrier_count = barrier_count + 1;
	if (barrier_count == num_costumers)
	{
		// if this is the last thread to reach the barrier release all.
		ret_val = ReleaseSemaphore(barrier_semaphore, num_costumers, NULL);
		if (FALSE == ret_val)
		{
			printf("Error when releasing barrier_semaphore\n");
			ret = ERROR_CODE;
		}

	}
	// up(mutex)
	ret_val = ReleaseMutex(barrier_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing barrier_mutex\n");
		return ERROR_CODE;
	}

	// if the up barrier_sem failed, return ERROR_CODE
	if (ret == ERROR_CODE)
		return ERROR_CODE;

	// down(barrier)
	wait_code = WaitForSingleObject(barrier_semaphore, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for barrier_semaphore\n");
		return ERROR_CODE;
	}
	return 0;
}

int tryToEnterTheRoom(costumer* costumer,hotel* hotel)
{
	DWORD wait_code;
	BOOL ret_val;
	int room_index = costumer->my_room;

	wait_code = WaitForSingleObject(hotel->rooms_sem[room_index], INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for the %d room semaphore\n",room_index);
		return ERROR_CODE;
	}

	return 0;
}

int writeToFile(costumer* costumer,hotel* hotel, int in_out)
{

	DWORD wait_code;
	BOOL ret_val;

	FILE* fp = NULL;
	int room_index = costumer->my_room;
	char* room_name, *costumer_name;
	int ret = 0, error_flag = 0;

	/* extract needed variables */
	room_index = costumer->my_room;
	room_name = hotel->room_names[room_index];
	costumer_name = costumer->name;

	wait_code = WaitForSingleObject(file_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for file_mutex\n");
		ret = ERROR_CODE;
		goto err0;
	}
	/*** critical Regan ***/


	//opening roomLog.txt file
	error_flag = fopen_s(&fp, "roomLog.txt", "a");
	if (fp == NULL)// check fopen failure 
	{
		printf("Error opening File - roomLog.txt\n");
		ret = ERROR_CODE;
		goto err1;
	}
	if (in_out == ENTER)
	{
		error_flag  = fprintf_s(fp,"%s %s IN %d\n", room_name, costumer_name, day);
		if (error_flag < 0)
		{
			printf("Error when writing to roomLog.txt\n");
			ret = ERROR_CODE;
		}
	}

	if (in_out == EXIT)
	{
		error_flag = fprintf_s(fp,"%s %s OUT %d\n", room_name, costumer_name, day);
		if (error_flag < 0)
		{
			printf("Error when writing to roomLog.txt\n");
			ret = ERROR_CODE;
		}
	}
	/* close file */
	fclose(fp);

err1: /* exit critical section */
	ret_val = ReleaseMutex(file_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing file_mutex\n");
		ret  = ERROR_CODE;
	}
err0:
	return ret;
}

int fillOutDay(costumer* costumer,hotel* hotel)
{
	DWORD wait_code;
	BOOL ret_val;
	int room_index, room_price, costumer_money, total_days, out_day;
	/* extract variables */ 
	room_index = costumer->my_room;
	costumer_money = costumer->money;
	room_price = hotel->price_per_person[room_index];

	/* calculate exit day*/
	total_days = costumer_money / room_price;

	wait_code = WaitForSingleObject(count_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for count_mutex\n");
		return ERROR_CODE;
	}
	/*** critical section ***/

	// find the exit day
	out_day = total_days + day;

	/* fill the out_days array*/
	out_days[costumer->index] = out_day;

	//the costumer entered the room, therefore not in waiting list
	rooms_waiting_list[costumer->my_room]--;

	/* exit critical section */
	ret_val = ReleaseMutex(count_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing count_mutex\n");
		return ERROR_CODE;
	}
	return 0;
}

int checkEndOfDay(costumer* costumer,hotel* hotel)
{
	DWORD wait_code;
	BOOL ret_val;
	int ret = 0;

	wait_code = WaitForSingleObject(count_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for count_mutex\n");
		return ERROR_CODE;
	}
	/*** critical Regan ***/

	count++;


	//if all the costumers that entered the room fill out the days array, call god to move the day
	if (count == num_people_entring_today)
	{
		ret_val = ReleaseSemaphore(god_signal,1,NULL);
		if (FALSE == ret_val)
		{
			printf("Error when signaling god_signal\n");
			ret = ERROR_CODE;
			goto release_mutex;
		}
	}
release_mutex:
	ret_val = ReleaseMutex(count_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing count_mutex\n");
		return ERROR_CODE;
	}
	return ret;
}

int accommodateRoom(costumer* costumer,hotel* hotel)
{
	DWORD wait_code;
	BOOL ret_val;
	int my_index;
	my_index = costumer->index;

	wait_code = WaitForSingleObject(checkout[my_index], INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for %d checkout mutex\n");
		return ERROR_CODE;
	}
}

int freeRoom(costumer* costumer,hotel* hotel)
{
	BOOL ret_val;
	int room_index = costumer->my_room;
	ret_val = ReleaseSemaphore(hotel->rooms_sem[room_index],1,NULL);
	if (FALSE == ret_val)
	{
		printf("Error when releasing  %d room semaphore\n", room_index);
		return ERROR_CODE;
	}
	return 0;
}

