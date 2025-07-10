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

func main() : Void {
  val created, clone : Frac = create_rational(1,2);
  clone = created;
  print(compare_rational(created, clone));
  print(add_rational(created, clone));
  print(sub_rational(created, clone));
  print(mul_rational(created, clone));
  print(div_rational(created, clone));
  print(neg_rational(created));
  print(inv_rational(created));
}
