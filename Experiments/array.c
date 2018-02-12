// experiment to explore writting past array bound
// Prof. Boothe - Jan 2017
#include <stdio.h>
#include <stdlib.h>
void arraytest();

#define NUM 11  // number to change

int main()
{
  int exit_status = 0;
  arraytest();
  printf("Exiting from main. Exit status = %d\n", exit_status);
  exit(exit_status);
}

void arraytest()
{
  int i, a[7];

  printf("         old value    new value\n");
  for (i=0; i < NUM; i++) {
    int orig_value = a[i];
    a[i] = 0;                  // init to 0  
    int new_value = a[i];
    printf("a[%d] = %11d, %11d\n", i, orig_value, new_value);
  }
}

