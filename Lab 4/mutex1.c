#include <stdio.h>
#include <pthread.h>

int global_var = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void *inc_gv(void *ptr)
{
	char *str;
	str = (char *) ptr;
	int i, j;
	for (i=0;i<10;i++)
	{
		pthread_mutex_lock( &mutex ); 		
		global_var ++;
		for (j=0;j<5000000;j++);
		printf("\n%s:", str);
		printf("%d.", global_var);
		pthread_mutex_unlock( &mutex );	
		fflush(stdout);
	}
}

int main ()
{
	pthread_t threadA, threadB;
	int tAret, tBret;

	char * str1 = "tA";
	char * str2 = "tB";

//	tAret = pthread_create(&threadA, NULL, inc_gv, NULL);
//	tBret = pthread_create(&threadB, NULL, inc_gv, NULL);

	tAret = pthread_create(&threadA, NULL, inc_gv, (void *) str1 );	
	tBret = pthread_create(&threadB, NULL, inc_gv, (void *) str2);
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);
	printf("\nCompleted!\n");
	pthread_mutex_destroy( &mutex );	
	return 0;
}



