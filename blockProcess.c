/*
 * blockProcess.c
 *
 *  Created on: May 13, 2017
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
#include <sys/file.h>

int main(int argc, char **argv) {

	int fd = open("textFile.txt", O_RDWR);

	while (fd < 0)
	{
		fd = open("textFile.txt", O_RDWR);
		printf("Error: could not open file\n");
		sleep(1);
	}

	printf("succesful open, file descriptor %d\n", fd);

	if (flock(fd, LOCK_EX) < 0)
	{
		printf("error flock %s\n", strerror(errno));
		return -1;
	}

	printf("Succesful file open,  from process with pid: %d\n", getpid());
	sleep(15);
}


