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

int excludeHidden(const struct dirent *dire);

int main(int argc, char *argv[])
{
	
	char *courseName = malloc(70);
	char *assignmentName = malloc(40);
	DIR *submitDir = malloc(512);

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
		char *response;
		printf("\nA previous submission with this assignment name exists.\nNew files will be added to that submission.\nYou may resubmit files, updating/replacing the old versions.\nExisting files which are not updated will not be affected.\nCurrent this submission contains:\n");
		//TO DO: print current stuff
		printf("\nDo you wish to continue? (enter 'yes' or 'no')\nUpdate previous submission: ");
		scanf("%s", response);
		if (strcmp(response, "yes")) {
			printf("\nSubmission aborted.\n");
			exit(1);
		}
	}
	else if (mkdir(courseName, 0644) < 0)
	{
		printf("Error creating assignment directory.");
	}

	// display files in current directory
//	int result = scandir(".", &namelist, excludeHidden, alphasort);
	printf("%10s %-6s %-5s %-25s", "SIZE", "DATE", "TIME", "NAME");
	for (i = 0; i < result; i++) {
		char tempDate[7];
		char tempTime[6];

//		stat(namelist[i]->d_name, &statStruct);
		strftime(tempDate, 6, "%b %d", localtime(&(statStruct.st_ctime)));
		printf("HERE");
		printf("%10d %-6s %-5s %-25s", (int)statStruct.st_size, tempDate, "", "");
	}

	return 0;
}

// filter for scandir to exclude hidden files
int excludeHidden(const struct dirent *dire)
{
	return dire->d_name[0] != '.';
}
