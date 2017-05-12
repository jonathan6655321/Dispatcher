/*
 * simpleProcess.c
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	printf("I am in a simple process %s\n", argv[0]);
	return 1;
}

