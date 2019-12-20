// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Costumer.h"

DWORD Costumer_thread(LPSTR lpParam)
{
	Costumer_arg *parameters = (Costumer_arg *)lpParam;
	costumer* costumer;
	hotel* hotel;

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
	costumer = parameters->costumer;
	hotel	 = parameters->hotel;
	num_costumers = parameters->N_costumers;

	ret_val =  firstDayFill(costumer,hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	/* barrier before first day starts */
	ret_val = preFirstDayBarrier(num_costumers);
	if (ret_val == ERROR_CODE) return ERROR_CODE;
	
	/* ---ALL THREADS STARTS TOGETHER HERE--- */

	/* try to enter the room if room is full wait until someone gets out */
	ret_val = tryToEnterTheRoom(costumer,hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	/* -- Entered the room --*/
	ret_val = writeToFile(costumer, hotel, IN);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = fillOutDay(costumer, hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;


	ret_val = checkEndOfDay(costumer, hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = accommodateRoom(costumer, hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	ret_val = writeToFile(costumer, hotel, OUT);
	if (ret_val == ERROR_CODE) return ERROR_CODE;


	ret_val = freeRoom(costumer, hotel);
	if (ret_val == ERROR_CODE) return ERROR_CODE;
}

int firstDayPreperation(costumer* costumer, hotel* hotel)
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
			return ERROR_CODE;
		}
	}
	// up(mutex)
	ret_val = ReleaseMutex(barrier_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing barrier_mutex\n");
		return ERROR_CODE;
	}
	// down(barrier)
	wait_code = WaitForSingleObject(barrier_semaphore, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for barrier_semaphore\n");
		return ERROR_CODE;
	}
	return 0;
}

int tryToEnterTheRoom(costumer* costumer, hotel* hotel)
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

int writeToFile(costumer* costumer, hotel* hotel, int in_out)
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
	error_flag = fopen_s(&rooms_file, "roomLog.txt", "w");
	if (rooms_file == NULL)// check fopen failure 
	{
		printf("Error opening File - roomLog.txt\n");
		ret = ERROR_CODE;
		goto err1;
	}
	if (in_out == IN)
	{
		error_flag  = fprintf_s("%s %s IN %d\n", room_name, costumer_name, day);
		if (error_flag < 0)
		{
			printf("Error when writing to roomLog.txt\n");
			ret = ERROR_CODE;
		}
	
	}
	if (in_out == OUT)
	{
		error_flag = fprintf_s("%s %s OUT %d\n", room_name, costumer_name, day);
		if (error_flag < 0)
		{
			printf("Error when writing to roomLog.txt\n");
			ret = ERROR_CODE;
		}
	}
	/* close file */
	fclose(fp);

err1: /* exit critical section */
	ret_val = ReleaseMutex(a_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing a_mutex\n");
		ret  = ERROR_CODE;
	}
err0:
	return ret;
}

int fillOutDay(costumer* costumer, hotel* hotel)
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
	out_day = total_days + day;

	/* fill the out_days array*/
	out_days[costumer->index] = out_day;

	return 0;
}

int checkEndOfDay(costumer* costumer, hotel* hotel)
{
	DWORD wait_code;
	BOOL ret_val;

	wait_code = WaitForSingleObject(count_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for count_mutex\n");
		return ERROR_CODE;
	}
	/*** critical Regan ***/

	count++;
	rooms_waiting_list[costumer->my_room]--;
	if (count == num_people_entring_today)
	{
		ret_val = ReleaseMutex(god_signal);
		if (FALSE == ret_val)
		{
			printf("Error when signaling god_signal\n");
			return ERROR_CODE;
		}
	}
	ret_val = ReleaseMutex(count_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing count_mutex\n");
		return ERROR_CODE;
	}
	return 0;
}

int accommodateRoom(costumer* costumer, hotel* hotel)
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

int freeRoom(costumer* costumer, hotel* hotel)
{
	BOOL ret_val;
	int room_index = costumer->index;
	ret_val = ReleaseMutex(hotel->rooms_sem[room_index]);
	if (FALSE == ret_val)
	{
		printf("Error when releasing  %d room semaphore\n", room_index);
		return ERROR_CODE;
	}
	return 0;
}

