import example;

val my_int = 10;
val x : Int = 10;
val s : String;
val w,y,z : Int;
w = 10 * 3;

func fib(val n : Int) : Int{
    if (n <= 1) return n;
    return fib(x-1) + fib(x+1);
}

func main(): void {
    print(fib(my_int));
}
