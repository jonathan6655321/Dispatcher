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

#define COUNTER_NUM_ARGUMENTS 5



int main(int argc, char **argv)
{
	printf("In counter and offset is: %s\n", argv[4]);
	kill(getppid(), SIGUSR1);
	sleep(10);


	return 1;
}
