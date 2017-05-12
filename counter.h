/*
 * counter.h
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#ifndef COUNTER_H_
#define COUNTER_H_

#include "Dispatcher.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>


int parseCounterArguments(char *charToCount, int *fileToProcessDescriptor,
		ssize_t *startOffsetInFile, ssize_t *numCharsToProcess, int argc, char **argv);



#endif /* COUNTER_H_ */
