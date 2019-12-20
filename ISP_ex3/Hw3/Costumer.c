// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Costumer.h"

DWORD Costumer(LPSTR lpParam)
{
	Costumer_arg *parameters = (Costumer_arg *)lpParam;
	int my_room, index, my_room_size;
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
	my_room = parameters->costumer->my_room;
	index = parameters->costumer->index;
	my_room_sem = parameters->hotel->rooms_sem[my_room];
	my_room_size = parameters->hotel->rooms_size[my_room];

	/* find the total number */
	ret_val =  firstDayFill(my_room, my_room_size);
	if (ret_val == ERROR_CODE) return ERROR_CODE;

	/* barrier before first day starts */
	ret_val = preFirstDayBarrier();
	if (ret_val == ERROR_CODE) return ERROR_CODE;
	
	/* ---ALL THREADS STARTS TOGETHER HERE--- */

	/* try to enter the room if room is full wait until someone gets out */
	ret_val = tryToEnterTheRoom(my_room, my_room_sem);
	if (ret_val == ERROR_CODE) return ERROR_CODE;




}


int firstDayPreperation(int room_index, int room_size)
{

	DWORD wait_code;
	BOOL ret_val;

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
}


int preFirstDayBarrier()
{
	DWORD wait_code;
	BOOL ret_val;
	
	// down(mutex)
	wait_code = WaitForSingleObject(barrier_mutex);
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
	wait_code = WaitForSingleObject(barrier_semaphore);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for barrier_semaphore\n");
		return ERROR_CODE;
	}

}

int tryToEnterTheRoom(int room_index, HANDLE room_semaphore)
{
	DWORD wait_code;
	BOOL ret_val;

	wait_code = WaitForSingleObject(room_semaphore);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for the %d room semaphore\n",room_index);
		return ERROR_CODE;
	}

}