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


ssize_t getFileSize(char *filePath);
ssize_t getSquareRootOfFileSize(ssize_t fileSize);
void my_signal_handler (int signum);



#endif /* DISPATCHER_H_ */
