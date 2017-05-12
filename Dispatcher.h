/*
 * Dispatcher.h
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "Dispatcher.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


int parseArguments(char *c, int *fileDescriptor, int argc, char **argv);
ssize_t getFileSize(int fileDescriptor);
ssize_t getSquareRootOfFileSize(ssize_t fileSize);



#endif /* DISPATCHER_H_ */
