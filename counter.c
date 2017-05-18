/*
 * counter.c
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#include "counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "Dispatcher.h"

#define COUNTER_NUM_ARGUMENTS 5



int main(int argc, char **argv)
{
	if(argc != COUNTER_NUM_ARGUMENTS)
	{
		printf("Error: wrong number of arguments for counter\n");
		return -1;
	}

	long pid = getpid();
	char pipePathName[MAX_PATH_LENGTH];
	sprintf(pipePathName, "/tmp/counter_%ld", pid);


	char charToCount = *argv[1];
	char* filePath = argv[2];
	char* numCharsToProcess = argv[3];
	char* offsetInFile = argv[4];


	int numCharInstancesInFile = countCharInstancesInFileSegment(charToCount, filePath, numCharsToProcess, offsetInFile );
	if(numCharInstancesInFile < 0)
	{
		return -1;
	}

	char message[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
	sprintf(message, "%d", numCharInstancesInFile);

	if(kill(getppid(), SIGUSR1) < 0)
	{
		printf("Error: kill failed %s\n", strerror(errno));
		return -1;
	}

	int pipeFileDescriptor = open(pipePathName, O_WRONLY | O_NONBLOCK);
	while (pipeFileDescriptor < 0)
	{
		pipeFileDescriptor = open(pipePathName, O_WRONLY | O_NONBLOCK);
	}

	ssize_t numBytesWritten = write(pipeFileDescriptor, message, MAX_DIGITS_TO_REPRESENT_FILE_SIZE);
	while( numBytesWritten < 0)
	{
		numBytesWritten = write(pipeFileDescriptor, message, MAX_DIGITS_TO_REPRESENT_FILE_SIZE);
	}

	close(pipeFileDescriptor); // Unmap the file, close the pipe, delete the pipe file. Exit.
	unlink(pipePathName);
//	sleep(1);
	return 1;
}

int countCharInstancesInFileSegment(char charToCount, char *filePath, char *numCharsToProcess, char *offsetInFile )
{
	int fileDescriptor = open(filePath, O_RDWR);
	if (fileDescriptor < 0)
	{
//		fileDescriptor = open(*filePath, O_RDWR);
		printf("%s\n", strerror(errno));
		return -1;
	}

	if (flock(fileDescriptor, LOCK_EX) < 0) // waits until other processes are done
	{
		printf("error flock %s\n", strerror(errno));
		return -1;
	}

	ssize_t numCharsToProcessSSIZE_T;
	ssize_t offsetInFileSSIZE_T;

	sscanf(numCharsToProcess, "%zd", &numCharsToProcessSSIZE_T);
	sscanf(offsetInFile, "%zd", &offsetInFileSSIZE_T);

	if(lseek(fileDescriptor, offsetInFileSSIZE_T, SEEK_SET) < 0)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	char *fileSegment = (char *)malloc(sizeof(char)*numCharsToProcessSSIZE_T);

	if(read(fileDescriptor, fileSegment, numCharsToProcessSSIZE_T) != numCharsToProcessSSIZE_T)
	{
		printf("%s\n", strerror(errno));
		return -1;
	}

	if (flock(fileDescriptor, LOCK_UN) < 0) // waits until other processes are done
	{
		printf("error flock %s\n", strerror(errno));
		return -1;
	}
//	printf("%s", fileSegment);

	int numCharInstances = 0;
	int i;
	for(i=0; i < numCharsToProcessSSIZE_T; i++)
	{
		if(fileSegment[i] == charToCount)
		{
			numCharInstances++;
		}
	}


	close(fileDescriptor);
	free(fileSegment);

	return numCharInstances;
}
