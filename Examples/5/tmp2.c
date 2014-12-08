#include <stdio.h>

struct X {
  int x;
  int y;
  double j[10];
};

int main() {
  struct X x;
  x.x=5;
  x.y=10;

  x.j[5] = 3.1;

  x.x=25;
  if (x.y>100)
    x.x -= 2;

  printf("%d %d\n",x.x,x.y);
  
  return 0;
}
