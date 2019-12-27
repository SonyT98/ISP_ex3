// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 3 in the Course Introduction to system programming
// This is the functions file that include all the main functions for the project.


// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Functions.h"
#include "Costumer.h"
#include "GlobalVariables.h"

// Function Definitions --------------------------------------------------------

int GetHotel(hotel **our_hotel)
{
	//variables
	FILE *rooms_file = NULL;
	char file_line[MAX_LINE_LENGTH] = { 0 };
	int room_index = 0, i = 0;
	int price = 0, size = 0;
	int ret = 0, error_flag = 0;


	//opening room.txt file
	error_flag = fopen_s(&rooms_file, "rooms.txt", "r");
	// check fopen failure 
	if (rooms_file == NULL)
	{
		printf("Error opening File - rooms.txt\n");
		ret = ERROR_CODE;
		goto err_fopen1;
	}

	//create hotel
	*our_hotel = (hotel*)malloc(sizeof(hotel));
	if (*our_hotel == NULL)
	{
		printf("Error allocating memory for hotel struct\n");
		ret = ERROR_CODE;
		goto err_malloc_hotel;
	}

	//reading the rooms file and update the hotel structure
	while (fgets(file_line, MAX_LINE_LENGTH, rooms_file) != NULL)
	{
		error_flag = sscanf_s(file_line, "%s %d %d", (*our_hotel)->room_names[room_index],
			MAX_ROOM_NAME-1, &price, &size);

		if(error_flag == ERROR_CODE)
		{ 
			printf("Error sscanf for rooms file\n");
			ret = ERROR_CODE;
			goto err_sscanf;
		}

		(*our_hotel)->price_per_person[room_index] = price;
		(*our_hotel)->rooms_size[room_index] = size;

		room_index++;
	}

	//updating the number of rooms
	(*our_hotel)->number_of_rooms = room_index;

	//initialize the handle semaphores
	for (i = 0; i < MAX_NUM_ROOMS; i++)
		(*our_hotel)->rooms_sem[i] = NULL;

	if (ret != ERROR_CODE)
		goto err_malloc_hotel;

err_sscanf:
	free(*our_hotel);
err_malloc_hotel:
	fclose(rooms_file);
err_fopen1:
	return ret;
}


int GetCostumers(costumer ***our_costumers, int *n_costumers)
{
	//variables
	FILE *names_file = NULL;
	char file_line[MAX_LINE_LENGTH] = { 0 };
	int costumers_index = 0;
	int ret = 0, error_flag = 0;

	//opening names.txt file
	error_flag = fopen_s(&names_file, "names.txt", "r");
	// check fopen failure 
	if (names_file == NULL)
	{
		printf("Error opening File - names.txt\n");
		ret = ERROR_CODE;
		goto err_fopen1;
	}

	//create costumers array
	*our_costumers = (costumer**)malloc(MAX_NUM_COSTUMERS*sizeof(costumer*));
	if (*our_costumers == NULL)
	{
		printf("Error allocating memory for costumer array\n");
		ret = ERROR_CODE;
		goto err_malloc_costumer_array;
	}

	//reading the names file and update the costumers array
	while (fgets(file_line, MAX_LINE_LENGTH, names_file) != NULL)
	{
		// creating the costumer structure
		(*our_costumers)[costumers_index] = (costumer*)malloc(sizeof(costumer));
		if ((*our_costumers)[costumers_index] == NULL)
		{
			printf("Error allocating memory for costumer struct\n");
			ret = ERROR_CODE;
			goto err_malloc_costumer;
		}
		//scan the file line
		error_flag = sscanf_s(file_line, "%s %d", (*our_costumers)[costumers_index]->name, MAX_LINE_LENGTH-1,
			&((*our_costumers)[costumers_index]->money));

		if (error_flag == ERROR_CODE)
		{
			printf("Error sscanf for names file\n");
			ret = ERROR_CODE;
			free((*our_costumers)[costumers_index]);
			goto err_malloc_costumer;
		}

		//we will find the room of each costumer in another function
		(*our_costumers)[costumers_index]->my_room = 0;

		(*our_costumers)[costumers_index]->index = costumers_index;
		costumers_index++;
	}

	*n_costumers = costumers_index;
	goto err_malloc_costumer_array;

err_malloc_costumer:
	FreeArrayOfPointers((*our_costumers), costumers_index);
	free((*our_costumers));
err_malloc_costumer_array:
	fclose(names_file);
err_fopen1:
	return ret;
}


void FreeArrayOfPointers(costumer **arr, int index)
{
	int i = 0;
	for (i = 0; i < index; i++) 
	{
		free(arr[i]);
	}
}


int SemaphoreIntialize(hotel *our_hotel, int num_of_costumers)
{
	int i = 0, ret = 0, j = 0, m = 0;
	int num_of_rooms = our_hotel->number_of_rooms;

	//first we will initialize the rooms semaphore to their size (room size)
	for (i = 0; i < num_of_rooms; i++)
	{
		our_hotel->rooms_sem[i] = CreateSemaphore(NULL, our_hotel->rooms_size[i],
			our_hotel->rooms_size[i], NULL);
		if (our_hotel->rooms_sem[i] == NULL)
		{
			printf("Error creating semaphore\n");
			ret = ERROR_CODE;
			goto err_room_sem;
		}

	}

	/*------------------------Global Semaphores--------------------------*/
	//semaphore for the god thread.
	god_signal = CreateSemaphore(NULL,0,1, NULL);
	if (god_signal == NULL)
	{ printf("Error creating semaphore\n"); ret = ERROR_CODE; goto err_room_sem;}
	
	//semaphore for the thread barriers.
	barrier_semaphore = CreateSemaphore(NULL, 0, num_of_costumers+1, NULL);
	if (barrier_semaphore == NULL)
	{ printf("Error creating semaphore\n"); ret = ERROR_CODE; goto err_bar_sem; }

	//semaphore for the costumer checkout. each semaphore will start at 0, and will be release when
	//a costumer checkout from his room.
	for (m = 0; m < num_of_costumers; m++)
	{
		checkout[m] = CreateSemaphore(NULL, 0, 1, NULL);
		if (checkout[m] == NULL)
		{ 
			printf("Error creating semaphore\n");
			ret = ERROR_CODE;
			goto err_checkout_sem; 
		}
	}

	/*--------------------------Global Mutexs----------------------------*/

	//mutex for the barrier code.
	barrier_mutex = CreateMutex(NULL,FALSE,NULL);
	if (barrier_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_checkout_sem; }

	//mutex for writing to the roomLog file.
	file_mutex = CreateMutex(NULL, FALSE, NULL);
	if (file_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_file_mutex; }

	//mutex for shared resources of the thread.
	a_mutex = CreateMutex(NULL, FALSE, NULL);
	if (a_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_a_mutex; }

	//mutex for shared resources of the thread.
	count_mutex = CreateMutex(NULL, FALSE, NULL);
	if (count_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_count_mutex; }

	if (ret == 0)
		goto ret_goto;


err_count_mutex:
	CloseHandle(a_mutex);
err_a_mutex:
	CloseHandle(file_mutex);
err_file_mutex:
	CloseHandle(barrier_mutex);
err_checkout_sem:
	CloseHandle(barrier_semaphore);
	for (j = 0; j < m; j++)
		CloseHandle(checkout[j]);
err_bar_sem:
	CloseHandle(god_signal);
err_room_sem:
	for (j = 0; j < i; j++)
		CloseHandle(our_hotel->rooms_sem[j]);
ret_goto:
	return ret;
}


int CreateCostumersAndGodArg(hotel *our_hotel, costumer **our_costumers, int n_costumers,
	Costumer_arg ***c_arg, God_arg **g_arg)
{
	//variables
	int ret = 0, i = 0, j = 0;

	//allocate space for costumer_arg array
	(*c_arg) = (Costumer_arg**)malloc(n_costumers*sizeof(Costumer_arg*));
	if (*c_arg == NULL)
	{
		printf("Error allocating memory for costumer_arg struct\n");
		ret = ERROR_CODE;
		goto ret_goto;
	}

	//go over the costumer arguments array and fill it accordingly.
	for (i = 0; i < n_costumers; i++)
	{
		//allocate memory for each argument in the costumers array
		(*c_arg)[i] = (Costumer_arg*)malloc(sizeof(Costumer_arg));
		if ((*c_arg)[i] == NULL)
		{
			printf("Error allocating memory for costumer_arg structure\n");
			ret = ERROR_CODE;
			goto err_malloc_c_arg;
		}
		(*c_arg)[i]->costumer = our_costumers[i];
		(*c_arg)[i]->hotel = our_hotel;
		(*c_arg)[i]->N_costumers = n_costumers;
	}

	//allocate space for god_arg array
	(*g_arg) = (God_arg*)malloc(sizeof(God_arg));
	if (*g_arg == NULL)
	{
		printf("Error allocating memory for god_arg structure\n");
		ret = ERROR_CODE;
		goto err_malloc_c_arg;
	}

	//fill the god argument
	(*g_arg)->costumers = our_costumers;
	(*g_arg)->N_costumers = n_costumers;

	if (ret == 0)
		goto ret_goto;

err_malloc_c_arg:
	for (j = 0; j < i; j++)
		free((*c_arg)[j]);
	free((*c_arg));
ret_goto:
	return ret;
}


int FindMyRoom(hotel *our_hotel, costumer **our_costumers, int n_costumers)
{
	//variables
	int i = 0, j = 0;;
	int num_rooms = 0, room_price = 0;
	costumer *cur_costumer = NULL;

	num_rooms = our_hotel->number_of_rooms;
	
	//go over the costumes and find the room for each one
	for (i = 0; i < n_costumers; i++)
	{
		cur_costumer = our_costumers[i];
		for (j = 0; j < num_rooms; j++)
		{
			//for every costumer, find his specific room (based on his money and room price)
			room_price = our_hotel->price_per_person[j];
			if (cur_costumer->money % room_price == 0)
			{
				cur_costumer->my_room = j;
				break;
			}
		}
	}
	return 0;
}

void FreeHotel(hotel *our_hotel)
{
	free(our_hotel);
}

void FreeCostumers(costumer **our_costumers, int n_costumers)
{
	FreeArrayOfPointers(our_costumers, n_costumers);
	free(our_costumers);
}

void FreeMutexAndSemaphoresHandles(hotel* our_hotel, int n_costumers)
{
	int j = 0;

	/*------------------------------ Semaphores ---------------------------------------*/
	//free the room's semaphores
	for (j = 0; j < our_hotel->number_of_rooms; j++)
		//close handle of the rooms semaphores (from the shared hotel struct to all costumers)
		CloseHandle(our_hotel->rooms_sem[j]);

	/*----------------------------- Global Semaphores ---------------------------------*/
	CloseHandle(god_signal);
	CloseHandle(barrier_semaphore);
	for (j = 0; j < n_costumers; j++)
		CloseHandle(checkout[j]);

	/*----------------------------- Global Mutexs --------------------- ---------------*/
	CloseHandle(barrier_mutex);
	CloseHandle(file_mutex);
	CloseHandle(a_mutex);
	CloseHandle(count_mutex);
}

void FreeCostumersAndGodArg(Costumer_arg **c_arg, God_arg *g_arg)
{
	int j = 0;

	//free c_arg structure
	for (j = 0; j < g_arg->N_costumers; j++)
		free(c_arg[j]);

	//free c_arg array
	free(c_arg);

	//free g_arg structure
	free(g_arg);
}

int runHotel()
{
	//variables
	costumer **costumers = NULL;
	hotel *my_hotel = NULL;
	Costumer_arg **c_arg = NULL;
	God_arg *g_arg = NULL;
	char *path = NULL;
	int number_of_costumers = 0, err = 0, wait_res = 0, j = 0, ret =0;


	//fill up hotel structure according to the rooms.txt file
	err = GetHotel(&my_hotel);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto err0; }

	//fill up costumer structure array according to the names.txt file
	err = GetCostumers(&costumers, &number_of_costumers);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto freeHotel; }

	//Initialize the rooms semaphores and the global semaphores/mutes to the required values
	err = SemaphoreIntialize(my_hotel, number_of_costumers);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto freeHotelAndCostumers; }

	//for each costumer find his room
	err = FindMyRoom(my_hotel, costumers, number_of_costumers);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto freeHotelCostumersAndHandles; }

	// create the arguments for costumer thread and god thread
	err = CreateCostumersAndGodArg(my_hotel, costumers, number_of_costumers, &c_arg, &g_arg);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto freeHotelCostumersAndHandles; }

	//create the thread that write to roomLog
	err = CreateThreads(c_arg, g_arg);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto freeAll; }




freeAll:
	FreeCostumersAndGodArg(c_arg,g_arg);

freeHotelCostumersAndHandles :
	FreeMutexAndSemaphoresHandles(my_hotel,number_of_costumers);

freeHotelAndCostumers:
	FreeCostumers(costumers,number_of_costumers);

freeHotel:
	FreeHotel(my_hotel);

err0:
	return ret;
}