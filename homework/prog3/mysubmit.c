/***********************************************************
 *
 *	Author: Alexander Dean
 *
 *	mysubmit - copy files into a special directory to hold
 *			   the submission
 *
 ***********************************************************/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

int excludeHidden(const struct dirent *dire);
void displayDir(struct dirent **namelist, int numEntries);
int cp(char *source, char *dest);

int main(int argc, char *argv[])
{
	
	char *courseName = malloc(70);
	char *assignmentName = malloc(40);
	int numFiles;	//holds result of scandir

	struct dirent **namelist = malloc(512);
	struct stat statStruct;

	int i;	//iterator for for loops
	
	// get values from user for course name and assignment
	printf("Enter the name of the computer science course. eg: cos350\nCourse: ");
	scanf("%s", courseName);
	
	// check if course directory exists
	while (stat(courseName, &statStruct) != 0) {
		printf("%s: No such file or directory\ninput validation failed, try again\nCourse: ", courseName);
		scanf("%s", courseName);
	}
	
	// check if submit directory exists
	while (stat(strcat(courseName, "/submit"), &statStruct) < 0) {
		printf("%s: No submit directory exists\ninput validation failed, try again\nCourse: ", courseName);
		scanf("%s", courseName);
	}
	printf("\n\nEnter the name of the programming assignment.\nUse ONE word only, no spaces or slashes. eg: prog1\nAssignment: ");
	scanf("%s", assignmentName);
	
	// check if assignment directory already exists
	if (stat(strcat(strcat(courseName, "/"), assignmentName), &statStruct) == 0)
	{	
		char response[4];
		printf("\nA previous submission with this assignment name exists.\nNew files will be added to that submission.\nYou may resubmit files, updating/replacing the old versions.\nExisting files which are not updated will not be affected.\nCurrent this submission contains:\n");
		numFiles = scandir(courseName, &namelist, excludeHidden, alphasort);
		displayDir(namelist, numFiles);
		printf("\nDo you wish to continue? (enter 'yes' or 'no')\nUpdate previous submission: ");
		scanf("%s", response);
		if (strcmp(response, "yes")) {
			printf("\nSubmission aborted.\n");
			exit(1);
		}
	}
	else if (mkdir(courseName, 0777) < 0)
	{
		printf("Error creating assignment directory.");
	}

	stat(courseName, &statStruct);
	printf("%o\n", statStruct.st_mode);
	
	//print files that can be submitted
	printf("The files in your current directory are:\n");
	numFiles = scandir(".", &namelist, excludeHidden, alphasort);
	displayDir(namelist, numFiles);
	
	// get user specifications of files to submit
	getchar();
	printf("\nEnter the names of the files you wish to include in this submission.\n Separate names with spaces. You may also use wild cards such as *\nTo submit all files in the current direcotry, just enter *\n\nFiles: ");

	char *files[numFiles];	//stores user-specified files (only should ever be numFiles entries)
	char *fileLine = NULL;	// line of file names to submit
	size_t nBytes = 0;			//size of line (there is probably a smarter way to do this)
	getline(&fileLine, &nBytes, stdin); 	// gets next line of input
	files[0] = malloc(80);
	files[0] = strtok(fileLine, " \n");
	
	//get tokens first, then handle. Try array of strings, malloc each individually
	for (i = 0; i < numFiles; i++)
	{
		files[i] = malloc(80);
		files[i] = strtok(NULL, " \n");
		if (files[i] == NULL) break;
	}

	int j;
	for (j = 0; j < i; j++)
	{
		// if * is entered, copy all files (not directories) in working directory
		if (strcmp(files[j], "*") == 0)
		{
			for (i = 0; i < numFiles; i++)
			{
				stat(namelist[i]->d_name, &statStruct);
				if (S_ISREG(statStruct.st_mode)) cp(namelist[i]->d_name, courseName);
			}
			break;	// because no other value after this matters (all files have been copied anyway)
		}

		// iterate through each user specified file and ensure it's in the entry
		int k = 0;
		int matchFound = 0;
		for (k = 0; k < numFiles; k++)
		{
			if (strcmp(files[j], namelist[k]->d_name) == 0)
			{
				// only copy if not directory
				stat(files[j], &statStruct);
				if (S_ISREG(statStruct.st_mode))	cp(files[j], courseName);
				else fprintf(stderr, "%s - SKIPPING: mysubmit does not copy directories\n", files[j]);
				matchFound = 1;
				break;
			}
		}
		if (!matchFound) fprintf(stderr, "problem matching: %s\n", files[j]);
	}

	//submission complete, print files in submission
	printf("\nSubmission completed. These are the files in this submission:\n");
	numFiles = scandir(courseName, &namelist, excludeHidden, alphasort);
	displayDir(namelist, numFiles);

	//return zero because I'm a good little C program
	return 0;
}

// filter for scandir to exclude hidden files
int excludeHidden(const struct dirent *dire)
{
	return dire->d_name[0] != '.';
}

void displayDir(struct dirent **namelist, int numEntries)
{
	int i = 0;
	struct stat statStruct;

	printf("%10s %-6s %-5s %-25s\n", "SIZE", "DATE", "TIME", "NAME");
	for (i = 0; i <  numEntries; i++) {
		char tempDate[8];
		char tempTime[8];

		stat(namelist[i]->d_name, &statStruct);

		strftime(tempDate, 7, "%b %d", localtime(&(statStruct.st_ctime)));
		strftime(tempTime, 7, "%H:%M", localtime(&(statStruct.st_ctime)));
		printf("%10d %-6s %-5s %-25s\n", (int)statStruct.st_size, tempDate, tempTime, namelist[i]->d_name);
	}
}

int cp(char *source, char *dest)
{
	// allocate a string that can hold the full file name
	char *destFileName = malloc(strlen(source) + strlen(dest) + 1);
	strcpy(destFileName, dest);		//copy destination directory name into destination file name

	int sourceFd = open(source, O_RDONLY);
	struct stat statStruct;
	stat(source, &statStruct);
	
	// allocate buffer for size of file (getting it all in one read() because it's 2018 and we have memory)
	char *buffer = malloc(statStruct.st_size);

	//open destination file
	int destFd = open(strcat(strcat(destFileName, "/"), source), O_CREAT | O_RDWR, statStruct.st_mode);
	read(sourceFd, buffer, statStruct.st_size);
	write(destFd, buffer, statStruct.st_size);
	return 0;
}
