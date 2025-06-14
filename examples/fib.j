import example;

val my_int : Int = 10;
val x : Int = 10;
val s : String;
val w,y,z : Int;
w = 10 * 3;

func fib(n : Int) : Int {
    return fib(x-1) + fib(x+1);
}

func main(): Void {
    print(fib(my_int));
}
