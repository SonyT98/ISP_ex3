// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Costumer.h"
#include "God.h"

// Function Definitions --------------------------------------------------------

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id, void *arg_pointer)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\n");
		printf("Received NULL pointer\n");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\n");
		printf("Received NULL pointer\n");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		arg_pointer,     /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return NULL;
	}

	return thread_handle;
}


int CloseThreads(HANDLE* p_thread_handles, int last_thread)
{
	int return_num = 0;
	int i = 0, ret_val = 0;
	// Close thread handles
	for (i = 0; i < last_thread; i++)
	{
		if (p_thread_handles[i] != NULL)
		{
			ret_val = CloseHandle(p_thread_handles[i]);
			if (false == ret_val)
			{
				printf("Error when closing thread handle\n");
				return_num = ERROR_CODE;
			}
		}
		else
		{
			printf("Error when closing thread handle\n");
			return_num = ERROR_CODE;
		}
	}
	return return_num;
}


int CreateThreads(Costumer_arg **c_arg, God_arg *g_arg)
{
	// variables
	HANDLE p_thread_handles[MAX_NUM_COSTUMERS+1] = { NULL };
	DWORD p_thread_ids[MAX_NUM_COSTUMERS + 1] = { 0 };
	DWORD wait_code = 0;

	int i = 0, error_val = 0, exit_code = 0;
	int number_of_costumers = c_arg[0]->N_costumers;

	// create all costumers threads in a loop
	for (i = 0; i < number_of_costumers; i++)
	{
		p_thread_handles[i] = CreateThreadSimple((LPTHREAD_START_ROUTINE)Costumer_thread, 
			&p_thread_ids[i], (void*)c_arg[i]);
		if (p_thread_handles[i] == NULL)
		{
			error_val = CloseThreads(p_thread_handles, i);
			return ERROR_CODE;
		}
	}

	// create the god thread
	p_thread_handles[number_of_costumers] = CreateThreadSimple((LPTHREAD_START_ROUTINE)God_thread,
		&p_thread_ids[number_of_costumers], (void*)g_arg);
	if (p_thread_handles[number_of_costumers] == NULL)
	{
		error_val = CloseThreads(p_thread_handles, number_of_costumers+1);
		return ERROR_CODE;
	}


	// wait for all thread to finish
	wait_code = WaitForMultipleObjects(number_of_costumers+1, p_thread_handles, TRUE, INFINITE);

	if (wait_code != WAIT_OBJECT_0)
	{
		printf("Error when waiting\n");
		error_val = CloseThreads(p_thread_handles, number_of_costumers + 1);
		return ERROR_CODE;
	}

	// get the exit codes
	for (i = 0; i < number_of_costumers +1; i++)
	{
		// get exit code into a temporary variable
		error_val = GetExitCodeThread(p_thread_handles[i], &exit_code);
		// check if GetExitCodeThread error
		if (error_val == 0)
		{
			printf("Error getting exit code from thread\n");
			error_val = CloseThreads(p_thread_handles, number_of_costumers + 1);
			return ERROR_CODE;
		}
		// check if thread error
		else if (exit_code == ERROR_CODE)
		{
			error_val = CloseThreads(p_thread_handles, number_of_costumers + 1);
			return ERROR_CODE;
		}
	}
	//close all the threads
	if (CloseThreads(p_thread_handles, number_of_costumers + 1) == ERROR_CODE)
		return ERROR_CODE;

	return 0;
}