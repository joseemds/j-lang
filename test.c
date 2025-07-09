#include <stdio.h>
#include "rational.h"
  int is_sum_even(int x[], int y[]){
  if ((x + y % 2 == 0)) goto label_0;
  return x % 2 == 0;
goto label_1;
label_0:
  return 1;
label_1:

}
  int main(){
  int zeros[] = {0, 0, 0, 0};
  return is_sum_even(1, 2);

}
