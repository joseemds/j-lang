#pragma once

#include "jmath.h"

typedef struct rational {
    int top;
    int bot;
} rational;

rational rational_add(rational a, rational b);
rational rational_sub(rational a, rational b);

rational rational_mul(rational a, rational b);

// rational rational_div(rational a, rational b){
//     rational r;
//
//     r.top = a.top * b.bot; 
//     r.bot = a.bot * b.top;
//
// 		return r
// }

void rational_simplify(rational* r);

float rational_divide(rational r);
