#include "rational.h"

rational rational_add(rational a, rational b){
    rational r;

    r.bot = lcm(a.bot, b.bot);

    r.top = a.top*(r.bot / a.bot) + b.top*(r.bot / b.bot);

    rational_simplify(&r);

    return r;
}

rational rational_sub(rational a, rational b){
    rational r;

    r.bot = lcm(a.bot, b.bot);

    r.top = a.top*(r.bot / a.bot) - b.top*(r.bot / b.bot);
    
    rational_simplify(&r);

    return r;
}

rational rational_mul(rational a, rational b){
    rational r;

    r.bot = a.bot * b.bot;

    r.top = a.top * b.top;

    rational_simplify(&r);

    return r;
}

// rational rational_div(rational a, rational b){
//     rational r;
//
//     r.top = a.top * b.bot; 
//     r.bot = a.bot * b.top;
//
// 		return r
// }

rational rational_square(rational a){
  rational r;

  r.top = square(a.top);

  r.bot = square(a.bot);

  rational_simplify(&r);

  return r;
}

rational rational_power(rational b, int p){
  rational r;

  r.top = power(b.top, p);
  
  r.bot = power(b.bot, p);

  rational_simplify(&r);

  return r;
}

void rational_simplify(rational* r){
    int common = gcd(r->top, r->bot);
    r->top /= common;
    r->bot /= common;
};

float rational_divide(rational r){
    return (float) r.top / r.bot; 
}
