#include "jmath.h"
#include <stdlib.h>

int gcd(int a, int b) {
  a = abs(a);
  b = abs(b);
  if (b == 0)
    return a;
  while (b != 0) {
    int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

int lcm(int a, int b) {
  if (a == 0 || b == 0)
    return 0;
  return abs(a * b) / gcd(a, b);
}

int max(int a, int b) { return (a > b) ? a : b; }

float square(float a) { return a * a; }

float power(float b, int p) {
  float v = 1;
  int abs_p = abs(p);

  for (int i = 0; i < abs_p; i++) {
    v *= b;
  }

  if (p < 0) {
    v = 1 / v;
  }

  return v;
}
int int_square(int a) { return (int)square((float)a); }

int int_power(int b, int p) { return (int)power((float)b, p); }
