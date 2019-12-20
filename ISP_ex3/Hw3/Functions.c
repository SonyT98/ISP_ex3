// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "Functions.h"

// Function Definitions --------------------------------------------------------

int GetHotel(hotel *our_hotel)
{
	//varibales
	FILE *rooms_file = NULL;
	char file_line[MAX_LINE_LENGTH] = { 0 };
	int room_price = 0;
	int room_index = 0;
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
	our_hotel = (hotel*)malloc(sizeof(hotel));
	if (our_hotel == NULL)
	{
		printf("Error allocating memory for hotel struct\n");
		ret = ERROR_CODE;
		goto err_malloc_hotel;
	}

	//create the string array of the rooms names
	our_hotel->room_names = (char**)malloc(MAX_NUM_ROOMS * sizeof(char*));
	if (our_hotel->room_names == NULL)
	{
		printf("Error allocating memory for hotel struct\n");
		ret = ERROR_CODE;
		goto err_malloc_room_names;
	}

	//create the array of prices for each room
	our_hotel->price_per_person = (int*)malloc(MAX_NUM_ROOMS * sizeof(int));
	if (our_hotel->price_per_person == NULL)
	{
		printf("Error allocating memory for hotel struct\n");
		ret = ERROR_CODE;
		goto err_malloc_room_prices;
	}

	//create the array of size for each room
	our_hotel->rooms_size = (int*)malloc(MAX_NUM_ROOMS * sizeof(int));
	if (our_hotel->rooms_size == NULL)
	{
		printf("Error allocating memory for hotel struct\n");
		ret = ERROR_CODE;
		goto err_malloc_room_size;
	}

	//reading the rooms file and update the hotel structure
	while (fgets(file_line, MAX_LINE_LENGTH, rooms_file) != NULL)
	{
		//create room name
		our_hotel->room_names[room_index] = (char*)malloc(MAX_NUM_ROOM_NAME*sizeof(char));
		if (our_hotel->room_names[room_index] == NULL)
		{
			printf("Error allocating memory for hotel struct\n");
			ret = ERROR_CODE;
			goto err_malloc_room_name_string;
		}

		sscanf_s(file_line, "%s %d %d", our_hotel->room_names[room_index], &(our_hotel->price_per_person[room_index]),
			&(our_hotel->rooms_size[room_index]), MAX_LINE_LENGTH);

		room_index++;
	}

err_malloc_room_name_string:
	FreeArrayOfPointers(our_hotel->room_names, room_index);
	free(our_hotel->rooms_size);
err_malloc_room_size:
	free(our_hotel->price_per_person);
err_malloc_room_prices:
	free(our_hotel->room_names);
err_malloc_room_names:
	free(our_hotel);
err_malloc_hotel:
	free(rooms_file);
err_fopen1:
	return ret;
}

int FreeArrayOfPointers(char **arr, int index)
{
	int i = 0;
	for (i = 0; i < index; i++) 
	{
		free(arr[i]);
	}
}