/*
 * counter.c
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#include "counter.h"


#define COUNTER_NUM_ARGUMENTS 5



int main(int argc, char **argv)
{
	printf("In counter and offset is: %s\n", argv[4]);
	kill(getppid(), SIGUSR1);
	sleep(10);

	return 1;
}
