#pragma once

#include "jmath.h"

typedef struct rational {
    int top;
    int bot;
} rational;

rational rational_add(rational a, rational b){
    rational r;

    r.bot = lcm(a.bot, b.bot);

    r.top = a.top*(r.bot / a.bot) + b.top*(r.bot / b.bot);

    return r;
}

rational rational_sub(rational a, rational b){
    rational r;

    r.bot = lcm(a.bot, b.bot);

    r.top = a.top*(r.bot / a.bot) - b.top*(r.bot / b.bot);

    return r;
}

rational rational_mul(rational a, rational b){
    rational r;

    r.bot = a.bot * b.bot;

    r.top = a.top * b.top;

    return r;
}

rational rational_div(rational a, rational b){
    rational r;

    r.top = a.top * b.bot; 
    r.bot = a.bot * b.top;
}

void rational_simplify(rational* r){
    int common = gcd(r->top, r->bot);
    r->top /= common;
    r->bot /= common;
};

float rational_divide(rational r){
    return (float)r.top / r.bot; 
}