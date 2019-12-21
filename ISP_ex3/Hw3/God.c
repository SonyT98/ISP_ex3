#include "God.h"
#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Costumer.h"



DWORD God_thread(LPSTR lpParam)
{
	God_arg *parameters = NULL;
	costumer* costumers;
	DWORD wait_code;
	BOOL ret_val;
	int num_costumers, num_left = 0, next_day, cur_room;

	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return ERROR_CODE;
	}

	/* Convert (void *) to parameters type */
	parameters = (God_arg *)lpParam;
	/* Convert the structure variable into local variables */
	num_costumers = parameters->N_costumers;
	costumers = *(parameters->costumers);

	while (num_left < num_costumers)
	{
		/* wait until the day ends */
		wait_code = WaitForSingleObject(god_signal, INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("Error when waiting for god_signal\n");
			return ERROR_CODE;
		}

		/* enter critical section */
		wait_code = WaitForSingleObject(count_mutex, INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("Error when waiting for count_mutex\n");
			return ERROR_CODE;
		}
		/*** critical section ***/

		next_day = findNextDay();
		day = next_day;
		num_people_entring_today = 0;
		for (int i = 0; i < num_costumers, i++)
		{
			cur_room = costumers[i].my_room;

			if ((out_days[i] == day) && (rooms_waiting_list[cur_room]) > 0)
			{
				num_people_entring_today++;
				ret_val = ReleaseMutex(checkout[i]);
				if (FALSE == ret_val)
				{
					printf("Error when releasing %d checkout mutex\n",i);
					return ERROR_CODE;
				}
				num_left++;
			
			}
		
		}
		ret_val = ReleaseMutex(count_mutex);
		if (FALSE == ret_val)
		{
			printf("Error when releasing count_mutex\n");
			return ERROR_CODE;
		}
	
	}

	return 0;
}



int findNextDay(int num_costumers)
{
	int min = 0, i = 0;
	while (out_days[i] == 0) i++;
	min = out_days[i];
	for (i; i < num_costumers; i++)
	{
		if ((out_days[i] < min)&& (out_days[i] > 0))
		{
			min = out_days[i];
		}
	}
	return min;
}