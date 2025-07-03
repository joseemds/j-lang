#include "jmath.h"

int gcd(int a, int b){
    int m = max(a,b);

    while(1){
      if(m % a == 0 && m % b == 0){
        return m;
      }
      m++;
    }
}

int lcm(int a, int b){
    return a*b / gcd(a,b);
}


int max(int a, int b){
    return (a > b) ? a : b;
}

float square(float a){
  return a * a;
}

float power(float b, int p){
  float v = 1;
  for(int i = 0; i < p; i++) v *= b;
  return v;
}