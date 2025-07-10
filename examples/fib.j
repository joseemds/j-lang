val my_int : Int = 10;
val x : Int = 10;
val s, s1, s2 : String;
val w, y, z : Int = 2, 3;
val a, b, c : Int = 1, 2, w;
val d,e,f : Int = x;
w = 10 * 3;

func fib(n : Int) : Int {
  return fib(x-1) + fib(x+1);
}

func fib_times_fib(n, m : Int) : Int {
  return fib(n) * fib(m);
}

func main() : Void {
  -- print(fib(my_int));
  val fib_5 : Int = fib(5);
}