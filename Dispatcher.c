#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "Dispatcher.h"

volatile int totalCharCount = 0;

int main(int argc, char **argv) {

	if (argc != DISPATCHER_NUM_ARGUMENTS) {
		printf("Error: wrong number of arguments for Dispatcher\n");
		return -1;
	}

	char charToCount = *argv[1];
	ssize_t fileSize = getFileSize(argv[2]);
	ssize_t numCharsPerProcess = getNumCharsPerProcess(fileSize); // file size might not divide equally into MAX_PROCESS_NUMBER

	char numCharsPerProcessString[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
	sprintf(numCharsPerProcessString, "%zd", numCharsPerProcess);

	// register the signal handler
	struct sigaction new_action;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = SA_SIGINFO;
	new_action.sa_sigaction = my_signal_handler;
	if (0 != sigaction(SIGUSR1, &new_action, NULL)) {
		printf("Signal handle registration failed. %s\n", strerror(errno));
		return -1;
	}

	// create the processes
	int i;
	for (i = 0; i < MAX_NUM_PROCESSES && (i * numCharsPerProcess < fileSize); i++) {

		int pid = fork();
		if (pid < 0) {
			printf("Error: fork failed: %s\n", strerror(errno));
			return -1;
		} else if (pid == 0) {

			// inside son process
			ssize_t offsetInFile = i * numCharsPerProcess;
			char offsetInFileString[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
			sprintf(offsetInFileString, "%zd", offsetInFile);

			char *argsForCounter[] = { "counter", &charToCount, argv[2],
					numCharsPerProcessString, offsetInFileString, NULL };

			if ((i + 1) * numCharsPerProcess > fileSize) {
				ssize_t numCharsToProcessLast = fileSize - (i) * numCharsPerProcess;
				char numCharsToProcessLastString[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
				sprintf(numCharsToProcessLastString, "%zd", numCharsToProcessLast);
				argsForCounter[3] = numCharsToProcessLastString;
			}
			execv("counter", argsForCounter);
			return -1;
		}
		sleep(2);
	}

	int status = 0; // TODO ?? should be zero?
	int childProcessWaitedFor;
	do {
		childProcessWaitedFor = wait(&status);
//		printf("%d FINISHED\n", childProcessWaitedFor);
	} while (childProcessWaitedFor != -1 || (childProcessWaitedFor == -1 && ECHILD != errno));

	printf("The count for char %c is: %d\n", charToCount, totalCharCount);
//	printf("%d\n", totalCharCount);
//	sleep(1);
	return 1;
}

ssize_t getFileSize(char *filePath) {
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0) {
		printf("Error: could not open file\n");
		return -1;
	}

	struct stat fileStat;
	if (fstat(fileDescriptor, &fileStat) < 0) {
		printf("Error: could not get file stats\n");
		return -1;
	}
	close(fileDescriptor);

	ssize_t fileSize = fileStat.st_size;
	return fileSize;
}

ssize_t getSquareRootOfFileSize(ssize_t fileSize) {
	ssize_t squareRoot = 0;
	while (squareRoot * squareRoot < fileSize) {
		squareRoot++;
	}
	return squareRoot;
}

ssize_t getNumCharsPerProcess(ssize_t fileSize) {
	if (fileSize < getpagesize() * 2) {
		return fileSize;
	} else {
		int i = 2;
		while (fileSize / i > getpagesize() * 2 && i < MAX_NUM_PROCESSES) {
			i++;
		}
		return fileSize / i + 1;
	}
}

void my_signal_handler(int signum, siginfo_t* info, void* ptr) {

	long signalSenderPid = (unsigned long) info->si_pid;

	char pipePathName[MAX_PATH_LENGTH];
	sprintf(pipePathName, "/tmp/counter_%ld", signalSenderPid);

	int pipeFileDescriptor = open(pipePathName, O_RDONLY);
	if (pipeFileDescriptor < 0) {
		printf("SIGNAL HANDLER: failed to open pipe\n");
	}

	// move this into an array that holds all pipes to try to read from
	char buf[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
	ssize_t numBytesRead = -1;
	while (read(pipeFileDescriptor, &buf, MAX_DIGITS_TO_REPRESENT_FILE_SIZE) <= 0)
	{
		printf("SIGNAL HANDLER: failed read from pipe\n");
	}

	ssize_t numCharCountInSegment;

	sscanf(buf, "%zd", &numCharCountInSegment);

	totalCharCount += numCharCountInSegment;
//
//	printf("The number of character received from %ld is: %s\n",
//			signalSenderPid % 16, buf);
}
