// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 3 in the Course Introduction to system programming
// This is the main function of the moed bet inn


/*oOoOoOoOoOoOoOoOoOoO Library Includes oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

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

/*oOoOoOoOoOoOoOoOoOoO Main Function oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int main(int argc, char** argv)
{
	//variables
	char *path = NULL;
	int err;

	// check number of input arguments
	if (argc != 2)
	{
		if (argc > 2) printf("Error: Too many arguments for Hw3\n");
		else printf("Error: Too few arguments for Hw3\n");
		return ERROR_CODE;
	}

	//change the directory folder to the required folder
	path = argv[1];
	err = chdir(argv[1]);
	if (err == ERROR_CODE)
	{
		printf("Error: folder path isn't valid!\n");
		return ERROR_CODE;
	}

	// run the hotel
	err = runHotel();
	if (err == ERROR_CODE) return ERROR_CODE;

	return 0;
}