#include <stdio.h>
#include <sys/types.h>
extern int64_t test_04(int64_t,int64_t,int64_t,int64_t);

int test_function(int x, int y, int z, int w)
{
  return x*y*z*w / w * w - x*x;
}

int main()
{
  
  int i, j, k;
  int errors=0;
  int success=0;

  for (i=-10; i<10; i++)
    for (j=-10; j<1; j++)
      for (k=-5; k<5; k++)
	if (test_04(i,j,k,3)!=test_function(i,j,k,3))
	  errors++;
	else
	  success++;

  printf("success,%d\nerrors,%d\ntotal,%d\n",success,errors,success+errors);

  return 0;
}
