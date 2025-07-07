#pragma once

typedef struct rational {
  int top;
  int bot;
} rational;

rational rational_add(rational a, rational b);

rational rational_sub(rational a, rational b);

rational rational_mul(rational a, rational b);

rational rational_div(rational a, rational b);

rational rational_square(rational a);

rational rational_power(rational b, int p);

rational rational_simplify(rational r);

int rational_compare(rational a, rational b);

float rational_divide(rational r);
