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

int excludeHidden(const struct dirent *dire);
void displayDir(struct dirent **namelist, int numEntries);

int main(int argc, char *argv[])
{
	
	char *courseName = malloc(70);
	char *assignmentName = malloc(40);

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
		int result = scandir(courseName, &namelist, excludeHidden, alphasort);
		displayDir(namelist, result);
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
	int result = scandir(".", &namelist, excludeHidden, alphasort);
	displayDir(namelist, result);
	
	// get user specifications of files to submit
	getchar();
	printf("\nEnter the names of the files you wish to include in this submission.\n Separate names with spaces. You may also use wild cards such as *\nTo submit all files in the current direcotry, just enter *\n\nFiles: ");

	char *files[result];

	char *fileLine = NULL;	// line of file names to submit
	size_t nBytes = 0;			//size of line (there is probably a smarter way to do this)
	i = 0;
	getline(&fileLine, &nBytes, stdin); 	// gets next line of input
	files[0] = malloc(80);
	files[0] = strtok(fileLine, " ");
	
	//get tokens first, then handle. Try array of strings, malloc each individually
	while (1)
	{
		i++;
		files[i] = malloc(80);
		files[i] = strtok(NULL, " ");
		if (files[i] == NULL) break;
	}

	printf("%d\n", i);
	int j;
	for (j = 0; j < i; j++)
	{
		printf("%d < %d\n", j, i);
		char *submitDir = strdup(courseName);
		//if (strcmp(files[j], "*") == 0)
		//{
			// copy all files (except hidden) to submit directory
		//	break;
		//}
		int k = 0;
		for (k = 0; k < result; k++)
		{
			printf("%s vs. %s\n", files[j], namelist[k]->d_name);
			if (strcmp(files[j], namelist[k]->d_name) == 0)
			{
				execl("/bin/cp", "/bin/cp", files[j], strcat(submitDir, "/"), (char*)NULL);
			}
		}
	}

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
