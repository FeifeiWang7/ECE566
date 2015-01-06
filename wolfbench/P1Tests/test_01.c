#include <stdio.h>
#include <sys/types.h>
extern int64_t test_01(int64_t,int64_t,int64_t);
int power(int base, int exp)
{
    int result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}
int test_function(int a, int b, int c)
{
  return power(a/c,4);
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-100; i<10; i++)
    for (j=10; j<12; j++)
      for (k=1; k<5; k++)
	if (test_01(i,j,-10)!=test_function(i,j,-10))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
