#include <stdio.h>
#include <sys/types.h>
extern int64_t test_02(int64_t,int64_t,int64_t);

int test_function(int a, int b, int c)
{
  return a*a*a+a;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-10; i<10; i++)
    for (j=-100; j<120; j++)
      for (k=-20; k<20; k++)
	if (test_02(i,j,633335)!=test_function(i,j,633335))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
