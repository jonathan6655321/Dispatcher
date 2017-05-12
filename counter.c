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
	long pid = getpid();
	char pipePathName[MAX_PATH_LENGTH];
	sprintf(pipePathName, "/tmp/counter_%ld", pid);

	printf("IN CHILD PIPE PATH NAME: %s\n", pipePathName);

	if (mkfifo(pipePathName, 0666) < 0)
	{
		printf("Error: failed to make the pipe\n");
	}

//	int pipeFileDescriptor = open(pipePathName, O_WRONLY);
//	if(pipeFileDescriptor < 0)
//	{
//		printf("Error: failed to open the pipe\n");
//		return -1;
//	}

	if(kill(getppid(), SIGUSR1) < 0)
	{
		printf("Error: kill failed %s\n", strerror(errno));
		return -1;
	}

//	if (write(pipeFileDescriptor, "a", 1) < 0)
//	{
//		printf("Error: failed to write into pipe\n");
//		return -1;
//	}
//
//	close(pipeFileDescriptor); // Unmap the file, close the pipe, delete the pipe file. Exit.
//	unlink(pipePathName);
	sleep(30);
	return 1;
}
