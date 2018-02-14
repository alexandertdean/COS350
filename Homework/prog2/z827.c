/*****************************************************
 *
 *	Author: Alexander Dean
 *
 *
 *
 *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char *argv[])
{
	// variable declaration
	int inputFd;// input file descriptor
	int outputFd;// output file descriptor
	char *outputFileName;
	int compress = 1;// set if file should be compressed, cleared if file should be decompressed

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		return 1;
	}

	// attempt to open input file
	inputFd = open(argv[1], O_RDWR);
	if (inputFd < 0)
	{
		perror("Error opening file");
		return 1;
	}
	outputFileName = strdup(argv[1]);
	
	// check if file is already compressed
	if (strstr(argv[1], ".z827"))
	{
		// output file is name with .z827 extension removed
		outputFileName[strlen(outputFileName) - 5] = 0;
		compress = 0;
	} 
	else 
	{
		// output file is name with .z827 extension appended
		strcat(outputFileName, ".z827");
		compress = 1;
	}

	// attempt to open output file
	outputFd = open(outputFileName, O_CREAT, 0644);
	if (outputFd < 0)
	{
		perror("Error opening output file");
		return 1;
	}

	// begin compression or decompression
	if (compress)	// compress the file
	{
		//TO DO: COMPRESSION CODE
	}
	else			// decompress the file 
	{
		//TO DO: DECOMPRESSION CODE
	}

	close(inputFd);
	return 0;
}
