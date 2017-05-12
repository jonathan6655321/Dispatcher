/*
 * counter.c
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#include "counter.h"


#define COUNTER_NUM_ARGUMENTS 5



int main(int argc, char **argv) {


	printf("I am here - counter.c\n");
	char charToCount;
	int fileDescriptor;
	ssize_t startOffsetInFile;
	ssize_t numCharsToProcess;



	if(parseCounterArguments(&charToCount, &fileDescriptor, &startOffsetInFile, &numCharsToProcess, argc, argv)
			< 0)
	{
		return -1;
	}

	printf("in counter.c and the start offset is: %zd", startOffsetInFile);

	return 1;
}

int parseCounterArguments(char *charToCount, int *fileDescriptor,
		ssize_t *startOffsetInFile, ssize_t *numCharsToProcess, int argc, char **argv)
{
	if (argc != COUNTER_NUM_ARGUMENTS)
	{
		printf("Error: wrong number of arguments for counter.c\n");
		return -1;
	}

	*fileDescriptor = open(argv[2], O_RDONLY);
	if(*fileDescriptor < 0)
	{
		printf("Error: could not open file\n");
		return -1;
	}

	*charToCount = *argv[1];
	*startOffsetInFile = *argv[3];
	*numCharsToProcess = *argv[4];


	return 1;
}
