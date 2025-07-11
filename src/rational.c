#include "rational.h"
#include "jmath.h"
#include <stdio.h>
#include <stdlib.h>

rational rational_create(int top, int bot) {
  if (bot == 0) {
    fprintf(stderr,
            "Not possible to create a rational with 0 as the denominator\n");
    exit(EXIT_FAILURE);
  }
  return (rational){.top = top, .bot = bot};
}

rational rational_add(rational a, rational b) {
  rational r;

  r.bot = lcm(a.bot, b.bot);

  r.top = a.top * (r.bot / a.bot) + b.top * (r.bot / b.bot);

  r = rational_simplify(r);

  return r;
}

rational rational_sub(rational a, rational b) {
  rational r;

  r.bot = lcm(a.bot, b.bot);

  r.top = a.top * (r.bot / a.bot) - b.top * (r.bot / b.bot);

  r = rational_simplify(r);

  return r;
}

rational rational_mul(rational a, rational b) {
  rational r;

  r.bot = a.bot * b.bot;

  r.top = a.top * b.top;

  r = rational_simplify(r);

  return r;
}

rational rational_div(rational a, rational b) {
  rational r;

  r.top = a.top * b.bot;
  r.bot = a.bot * b.top;

  r = rational_simplify(r);

  return r;
}

rational rational_square(rational a) {
  rational r;

  r.top = int_square(a.top);

  r.bot = int_square(a.bot);

  r = rational_simplify(r);

  return r;
}

rational rational_power(rational b, int p) {
  rational r;

  if (p < 0) {
    int temp = b.top;
    b.top = b.bot;
    b.bot = temp;
    p *= -1;
  }
  r.top = int_power(b.top, p);

  r.bot = int_power(b.bot, p);

  r = rational_simplify(r);

  return r;
}

rational rational_invert(rational a) { return rational_power(a, -1); }

rational rational_neg(rational a) {
  rational r;

  r.top = -a.top;

  r.bot = a.bot;

  return r;
}

rational rational_simplify(rational r) {
  int common = gcd(r.top, r.bot);
  r.top /= common;
  r.bot /= common;
  return r;
}

int rational_compare(rational a, rational b, int op) {
  a = rational_simplify(a);
  b = rational_simplify(b);
  int mmc = lcm(a.bot, b.bot);

  switch(op) {
    case 279: // CMP
    case 280: // NEQ
      return (a.top == b.top && a.bot == b.bot) ? 1 : 0;
      break;
    case 281: // GT
      a.top = a.top * (mmc / a.bot);
      b.top = b.top * (mmc / b.bot);

      return (a.top > b.top) ? 1 : 0;
      break;
    case 282: // GEQ
      a.top = a.top * (mmc / a.bot);
      b.top = b.top * (mmc / b.bot);

      return (a.top >= b.top) ? 1 : 0;
      break;
    case 283: // LT
      a.top = a.top * (mmc / a.bot);
      b.top = b.top * (mmc / b.bot);

      return (a.top < b.top) ? 1 : 0;
      break;
    case 284: // LEQ
      a.top = a.top * (mmc / a.bot);
      b.top = b.top * (mmc / b.bot);

      return (a.top <= b.top) ? 1 : 0;
      break;
  }
  return 0;
}

float rational_divide(rational r) { return (float)r.top / r.bot; }

float frac_to_decimal(rational r) { return (float)r.top / r.bot; }

rational int_to_frac(int x) {
  return rational_create(x, 1);
}

void print_rational(rational r) { printf("%d/%d", r.top, r.bot); }

rational rational_mul_scalar(rational a, int s) {
  rational r;
  r.top = a.top * s;
  r.bot = a.bot;

  return r;
}
