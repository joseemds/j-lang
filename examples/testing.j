func is_sum_even(x,y : Int) : Bool {
  if ((x + y) % 2 == 0){
    return true;
  } else {
    return x % 2 == 0;
  }
}



func main() : Bool {
	return is_sum_even(1,2);
}
