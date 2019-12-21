// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Functions.h"
#include "Costumer.h"
#include "GlobalVariables.h"

// Function Definitions --------------------------------------------------------

int GetHotel(hotel **our_hotel)
{
	//varibales
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
		sscanf_s(file_line, "%s %d %d", ((*our_hotel)->room_names)[room_index], MAX_LINE_LENGTH, 
			&((*our_hotel)->price_per_person[room_index]), &((*our_hotel)->rooms_size[room_index]));


		room_index++;
	}

	//updating the number of rooms
	(*our_hotel)->number_of_rooms = room_index;

	//intialize the hanlde semaphores
	for (i = 0; i < MAX_NUM_ROOMS; i++)
		(*our_hotel)->rooms_sem[i] = NULL;

err_malloc_hotel:
	fclose(rooms_file);
err_fopen1:
	return ret;
}


int GetCostumers(costumer ***our_costumers, int *n_costumers)
{
	//varibales
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
		// creating the costumer struct
		(*our_costumers)[costumers_index] = (costumer*)malloc(sizeof(costumer));
		if ((*our_costumers)[costumers_index] == NULL)
		{
			printf("Error allocating memory for costumer struct\n");
			ret = ERROR_CODE;
			goto err_malloc_costumer;
		}
		sscanf_s(file_line, "%s %d", (*our_costumers)[costumers_index]->name, MAX_LINE_LENGTH,
			&((*our_costumers)[costumers_index]->money));

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


int FreeArrayOfPointers(costumer **arr, int index)
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

	day = 6;

	//first we will intialize the rooms semaphore to their size
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
	god_signal = CreateSemaphore(NULL,0,1, NULL);
	if (god_signal == NULL)
	{ printf("Error creating semaphore\n"); ret = ERROR_CODE; goto err_room_sem;}
	
	barrier_semaphore = CreateSemaphore(NULL, 0, num_of_costumers, NULL);
	if (barrier_semaphore == NULL)
	{ printf("Error creating semaphore\n"); ret = ERROR_CODE; goto err_bar_sem; }


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

	barrier_mutex = CreateMutex(NULL,FALSE,NULL);
	if (barrier_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_checkout_sem; }

	file_mutex = CreateMutex(NULL, FALSE, NULL);
	if (file_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_file_mutex; }

	a_mutex = CreateMutex(NULL, FALSE, NULL);
	if (a_mutex == NULL)
	{ printf("Error creating mutex\n"); ret = ERROR_CODE; goto err_a_mutex; }

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

	//go over the costumer arg array and fill it accordinly.
	for (i = 0; i < n_costumers; i++)
	{
		(*c_arg)[i] = (Costumer_arg*)malloc(sizeof(Costumer_arg));
		if ((*c_arg)[i] == NULL)
		{
			printf("Error allocating memory for costumer_arg struct\n");
			ret = ERROR_CODE;
			goto err_malloc_c_arg;
		}
		(*c_arg)[i]->costumer = our_costumers[i];
		(*c_arg)[i]->hotel = our_hotel;
		(*c_arg)[i]->N_costumers = n_costumers;
	}

	//allocate space for costumer_arg array
	(*g_arg) = (God_arg*)malloc(sizeof(God_arg));
	if (*g_arg == NULL)
	{
		printf("Error allocating memory for god_arg struct\n");
		ret = ERROR_CODE;
		goto err_malloc_c_arg;
	}

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
	int i = 0, j = 0;;
	int num_rooms = 0, room_price = 0;
	costumer *cur_costumer = NULL;

	num_rooms = our_hotel->number_of_rooms;

	for (i = 0; i < n_costumers; i++)
	{
		cur_costumer = our_costumers[i];
		for (j = 0; j < num_rooms; j++)
		{
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