#include <stdio.h>
#include <math.h>
#include <sys/types.h>
extern int64_t test_00(int64_t,int64_t,int64_t);


int power(int base, int exp)
{
    int result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}
int test_function(int a, int b, int c)
{
  return 1/power(a,5);;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-5; i<10; i++)
{ if (i == 0) continue;
    for (j=1; j<12; j++)
      for (k=-10; k<10; k++)
	if (test_00(i,j,k)!=test_function(i,j,k))
	  errors++;
	else
	  success++;
}

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
