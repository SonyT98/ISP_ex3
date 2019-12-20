// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Functions.h"

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