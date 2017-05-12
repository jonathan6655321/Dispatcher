/*
 * Dispatcher.h
 *
 *  Created on: May 12, 2017
 *      Author: Jonathan
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define DISPATCHER_NUM_ARGUMENTS 3
#define MAX_DIGITS_TO_REPRESENT_FILE_SIZE 20 // much more than needed
#define MAX_PATH_LENGTH 100

ssize_t getFileSize(char *filePath);
ssize_t getSquareRootOfFileSize(ssize_t fileSize);
void my_signal_handler( int signum, siginfo_t* info, void* ptr);



#endif /* DISPATCHER_H_ */
