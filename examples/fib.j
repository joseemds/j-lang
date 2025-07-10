func fib(n : Int) : Int {
  if (n <= 1) { return n; }
  return fib(n-1) + fib(n-2);
}

func fib_times_fib(n, m : Int) : Int {
  return fib(n) * fib(m);
}

func main() : Void {
  val my_int : Int = 7;
  print(fib(my_int));
  val x : Int = 10;
  val s, s1, s2 : String;
  val w, y, z : Int = 2, 3;
  val a, b, c : Int = 1, 2, w;
  val d,e,f : Int = x;
  w = 10 * 3;
}