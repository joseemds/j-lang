func create_rational(a, b : Int) : Frac {
  val r : Frac = a // b;
  return r;
}

func compare_rational(a, b : Frac) : Bool {
  return a == b;
}

func add_rational(a, b : Frac) : Frac {
  return a + b;
}

func sub_rational(a, b : Frac) : Frac {
  return a - b;
}

func mul_rational(a, b : Frac) : Frac {
  return a * b;
}

func div_rational(a, b : Frac) : Frac {
  return a / b;
}

func neg_rational(a : Frac) : Frac {
  return -a;
}

func inv_rational(a : Frac) : Frac {
  return (1 // 1 / a);
}

func main() : Void {}