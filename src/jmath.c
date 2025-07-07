#include "jmath.h"

int gcd(int a, int b) {
  int m = max(a, b);

  while (1) {
    if (m % a == 0 && m % b == 0) {
      return m;
    }
    m++;
  }
}

int lcm(int a, int b) { return a * b / gcd(a, b); }

int max(int a, int b) { return (a > b) ? a : b; }

float square(float a) { return a * a; }

float power(float b, int p) {
  float v = 1;
  if (p < 0)
    b = 1 / b;
  for (int i = 0; i < p; i++)
    v *= b;
  return v;
}

int int_square(int a) { return (int)square((float)a); }

int int_power(int b, int p) { return (int)power((float)b, p); }