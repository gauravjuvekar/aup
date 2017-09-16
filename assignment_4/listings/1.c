#include<stdio.h>
#include <stdlib.h>
#include<string.h>

/* prints all environment variables and adds a new env variable, and verifies it*/

extern char **environ; //using the POSIX defined user environment variable 
int main(int argc, char *argv[], char** envp)
{
	int count = 0;
	char **env;
	printf("\n");
	for (env = envp; *env != 0; env++) 
	{
		char* thisEnv = *env;
		printf("%d. [%s] \n", count++, thisEnv);
	}

	int i = setenv("MYVAR", "IFTHEN", 1); //set the new env variable
	
	count = 0;

	printf("\n\n\n");

	while(environ[count] != NULL)
	{
		//prints the env variables with the new one added to the array
		printf("%d. [%s] \n ", count, environ[count]);
		count++;
	}	


	return 0;
}
