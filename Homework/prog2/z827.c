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
#include <sys/stat.h>


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
	outputFd = open(outputFileName, O_CREAT | O_WRONLY, 0644);
	if (outputFd < 0)
	{
		perror("Error opening output file");
		return 1;
	}

	// begin compression or decompression
	if (compress)	// compress the file
	{
		unsigned int buffer = 0; // bit buffer
		unsigned int numBits = 0; // number of bits in current byte
		unsigned char *inputBuf; //input file for System read function
		unsigned int bufSize = 0; // size of input buffer (to know when to dump to output file)
		unsigned int i;	// iterator for for loops, etc
		unsigned int readResult; // checks return value on read()
		unsigned char notEOF = 1; // true when eof is reached

		// get file size and print to file
		struct stat fileStats;
		stat(argv[1], &fileStats);
		unsigned int fileSize = fileStats.st_size;
		for (i = 0; i < 4; i++)
		{
			// print in little endian order
			write(outputFd, &fileSize, 1);
			fileSize = fileSize >> 8;		
		}

		readResult = read(inputFd, inputBuf, 1);	// get one character from input file to start (ensures file isn't empty)
		if (readResult < 0) 
		{
			fprintf(stderr, "Error reading file\n");
			exit(1);
		}
		else if (readResult < 1)
		{
			fprintf(stderr, "Cannot compress empty file\n");
			exit(1);
		}

		// compress rest of file
		while (notEOF)
		{
			for (numBits = 0; numBits < 7; numBits++)
			{
				buffer |= (0x01 & (*inputBuf >> numBits)) << bufSize;
				bufSize++;
				if (bufSize >= 8)
				{
					// dump bit buffer when it is greater than or equal to 8 bits
					write(outputFd, &buffer, 1);
					bufSize = 0;
					buffer = 0;
				}
			}
			
			// check last bit
			if (!((0x01 << numBits) | *inputBuf))	// 1 in MSB, file cannot be compressed
			{
				fprintf(stderr, "File cannot be compressed.");
				exit(1);
			}
		
			// get next character	
			readResult = read(inputFd, inputBuf, 1);
			if (readResult < 0)
			{
				fprintf(stderr, "Error in file reading\n");
				exit(1);
			}
			else if (readResult < 1)
			{
				notEOF = 0;
			}
		}
//		buffer = buffer << (8 - bufSize);
		write(outputFd, &buffer, 1);
			
	}
	else			// decompress the file 
	{
		//TO DO: DECOMPRESSION CODE
	}

	close(inputFd);
	close(outputFd);
	return 0;
}
