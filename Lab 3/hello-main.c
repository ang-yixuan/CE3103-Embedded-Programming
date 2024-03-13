#include "hello.h"
#include <stdio.h>
#include <unistd.h>

int main()
{
 
 printf("Hello World from main! \n");
 sleep(0x5fffff);
 int count=0;
 
 helloprint();
 sleep(0x6ffffff);
 
 count=countUp(count);
 sleep(0x7fffffff);
 
 printf("Bye! \n");
 return 0;
}
