#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>


#define DISPATCHER_NUM_ARGUMENTS 3


int main(int argc, char **argv) {

	if(argc != DISPATCHER_NUM_ARGUMENTS)
	{
		printf("Error: wrong number of arguments for Dispatcher\n");
		return -1;
	}

	char charToCount = *argv[1];

	ssize_t fileSize = getFileSize(argv[2]);
	ssize_t numCharsPerProcess = getSquareRootOfFileSize(fileSize);
}

ssize_t getFileSize(char *filePath)
{
	int fileDescriptor = open(*filePath, O_RDONLY);
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
	return fileStat.st_size;
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
