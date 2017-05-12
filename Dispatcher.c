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



int main(int argc, char **argv) {

	if(argc != DISPATCHER_NUM_ARGUMENTS)
	{
		printf("Error: wrong number of arguments for Dispatcher\n");
		return -1;
	}

	char charToCount = *argv[1];
	ssize_t fileSize = getFileSize(argv[2]);
	ssize_t numCharsPerProcess = getSquareRootOfFileSize(fileSize);

	char numCharsPerProcessString[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
	sprintf(numCharsPerProcessString, "%zd", numCharsPerProcess);


	  // Structure to pass to the registration syscall
	  struct sigaction new_action;
	  sigemptyset(&new_action.sa_mask);
	  // Remove any special flag
	  new_action.sa_flags = SA_SIGINFO;
	  // Assign pointer to our handler function
	  new_action.sa_sigaction = my_signal_handler;
	  // Register the handler
	  if (0 != sigaction (SIGUSR1, &new_action, NULL))
	  {
	    printf("Signal handle registration failed. %s\n",strerror(errno));
	    return -1;
	  }


	int i;
	for (i=0; i < 16; i++)
	{
		int pid = fork();
		if (pid < 0)
		{
			printf("Error: fork failed: %s\n", strerror(errno));
			return -1;
		}
		else if (pid == 0)
		{
			// inside son process
			ssize_t offsetInFile = i*numCharsPerProcess;
			char offsetInFileString[MAX_DIGITS_TO_REPRESENT_FILE_SIZE];
			sprintf(offsetInFileString, "%zd", offsetInFile);

			char *argsForCounter[] = {"counter", &charToCount, argv[2], numCharsPerProcessString,
					offsetInFileString, NULL};

			execv("counter", argsForCounter);
			printf("SHOULD NEVER GET HERE\n");
			return -1;
		}
		sleep(2);
	}

	int status;
	while(wait(&status) != -1);

	return 1;
}

ssize_t getFileSize(char *filePath)
{
	int fileDescriptor = open(filePath, O_RDONLY);
	if (fileDescriptor < 0)
	{
		printf("Error: could not open file\n");
		return -1;
	}

	struct stat fileStat;
	if(fstat(fileDescriptor, &fileStat) < 0)
	{
		printf("Error: could not get file stats\n");
		return -1;
	}
	close(fileDescriptor);

	ssize_t fileSize = fileStat.st_size;
	return fileSize;
}

ssize_t getSquareRootOfFileSize(ssize_t fileSize)
{
	ssize_t squareRoot = 0;
	while(squareRoot * squareRoot < fileSize)
	{
		squareRoot++;
	}
	return squareRoot;
}

void my_signal_handler( int signum, siginfo_t* info, void* ptr)
{
	long signalSenderPid = (unsigned long) info->si_pid;

	char pipePathName[MAX_PATH_LENGTH];
	sprintf(pipePathName, "/tmp/counter_%ld", signalSenderPid);
//	printf("IN PARENT signal handler: the pipes path name is: %s\n\n", pipePathName);

	if (mkfifo(pipePathName, 0666) < 0)
	{
		printf("Error: failed to make the pipe\n");
	}

	int pipeFileDescriptor = open(pipePathName, O_RDONLY | O_NONBLOCK);
	if(pipeFileDescriptor < 0)
	{
		printf("Error: failed to open pipe\n");
	}

//	printf("opened in father\n");

	char buf = 'c';
	ssize_t numBytesRead = -1;
	while(numBytesRead <= 0)
	{
		numBytesRead = read(pipeFileDescriptor, &buf, 1);
	}

	printf("The character received from %ld is: %c\n", signalSenderPid, buf);
}
