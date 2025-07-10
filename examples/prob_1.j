-- func prob_1_frac_type_error(x, y : Frac, c : Int) : Void {
-- val res : Frac;
--  res = (x*x) - y + c; -- here!
-- print(res);
-- }

func prob_1_frac_as_frac(x, y : Frac, c : Int) : Void {
  val res : Frac;
  res = (x*x) - y + int_to_frac(c);
  print(res);
}

func prob_1_frac_as_float(x, y : Frac, c : Int) : Void {
  val res : Float;
  res = (frac_to_decimal(x*x)) - frac_to_decimal(y) + int_to_float(c);
  print(res);
}

-- func prob_1_float_type_error(x, y : Float, c : Int) : Void {
-- val res : Float;
--  res = x*x - y + c; -- here!
--  print(res);
-- }

func prob_1_float(x, y : Float, c : Int) : Void {
  val res : Float;
  res = (x*x) - y + int_to_float(c);
  print(res);
}

func main() : Void {
	prob_1_frac_as_frac(1//2, 3//4, 1);
	prob_1_frac_as_float(1//2, 3//4, 1);
	val x : Float = 2.0;
	prob_1_float(0.5, 0.75, 1);
}
