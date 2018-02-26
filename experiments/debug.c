// program for experiment 5, using the debugger - Prof. Boothe  1/08
#include <stdio.h>
#include <stdlib.h>
void func(int callcnt);

int global = 0;

int main()
{
  func(1);
}

// recursive function that stops recursing at callcnt 3
void func(int callcnt)
{
  int local = callcnt;
  global = local + 10;
  int *dynamic = malloc(sizeof(int));
  *dynamic = global + 100;

  printf("starting call # %d\n", local);

  if (local < 3)
    func(local+1);
  
  printf("finishing call # %d\n", local);
  return;
}
