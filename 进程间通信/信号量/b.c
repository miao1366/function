#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem.h"
 
int main()
{
	sem_init();
	int i = 0;
	for(;i < 5;i++)
	{
		sem_p();
		printf("B");
		fflush(stdout);
		int n = rand() % 3;
		sleep(n);
		printf("B");
		fflush(stdout);
		
		sem_v();
		n = rand() % 3;
		sleep(n);
	}
}