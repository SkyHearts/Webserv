#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char** env)
{
    int i = 0;
    printf("In cgi printenv\n");
	while (env[i]){
            printf("%s\n", env[i]);
            i++;
        }
    printf("In cgi using getenv()\n");
    printf("PATH_INFO : %s\n", getenv("PATH_INFO"));
    printf("PORT : %s\n", getenv("SERVER_PORT"));
    printf("METHOD : %s\n", getenv("REQUEST_METHOD"));
	return 0;
}