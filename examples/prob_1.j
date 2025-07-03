func prob_1_frac_type_error(x, y : Frac, c : Int) : Void {
  val res : Frac;
  res = square(x) - y + c; -- here!
  print(res);
}

func prob_1_frac_as_frac(x, y : Frac, c : Int) : Void {
  val res : Frac;
  res = square(x) - y + toFrac(c);
  print(res);
}

func prob_1_frac_as_float(x, y : Frac, c : Int) : Void {
  val res : Float;
  res = square(toDecimal(x)) - toDecimal(y) + toFloat(c);
  print(res);
}

func prob_1_float_type_error(x, y : Float, c : Int) : Void {
  val res : Float;
  res = square(x) - y + c; -- here!
  print(res);
}

func prob_1_float(x, y : Float, c : Int) : Void {
  val res : Float;
  res = square(x) - y + toFloat(c);
  print(res);
}
