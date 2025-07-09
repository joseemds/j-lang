// Código gerado automaticamente pela J-Lang#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rational.h"
#include "vector.h"

  int is_sum_even(int x[], int y[]){
  if ((x + y % 2 == 0)) goto label_0;
  return x % 2 == 0;
goto label_1;
label_0:
  return 1;
label_1:

}
  int main(){
    Vector* zeros = vector_init(sizeof(int));
  int _tmp_zeros_0 = 0;
  vector_push(zeros, &_tmp_zeros_0);
  int _tmp_zeros_1 = 0;
  vector_push(zeros, &_tmp_zeros_1);
  int _tmp_zeros_2 = 0;
  vector_push(zeros, &_tmp_zeros_2);
  int _tmp_zeros_3 = 0;
  vector_push(zeros, &_tmp_zeros_3);
  return is_sum_even(1, 2);

}
