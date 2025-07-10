#pragma once

typedef struct rational {
  int top;
  int bot;
} rational;

rational rational_create(int top, int bot);

rational rational_add(rational a, rational b);

rational rational_sub(rational a, rational b);

rational rational_mul(rational a, rational b);

rational rational_mul_scalar(rational a, int b);

rational rational_div(rational a, rational b);

rational rational_square(rational a);

rational rational_power(rational b, int p);

rational rational_simplify(rational r);

rational rational_neg(rational a);

int rational_compare(rational a, rational b, int op);

float rational_divide(rational r);

float frac_to_decimal(rational r);

void print_rational(rational r);

rational int_to_frac(int x);
