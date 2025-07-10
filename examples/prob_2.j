func main () : Void {
  val till_25, till_50, till_75, till_100 : Int = 0, 0, 0, 0;
  while(true) {
    val x : Int = str_to_int(input());
    if (x < 0) { break; }
    if (x < 26) { till_25 = till_25 + 1; continue; }
    if (x < 51)  { till_50 = till_50 + 1; continue; }
    if (x < 76) { till_75 = till_75 + 1; continue; }
    if (x < 101) {till_100 = till_100 + 1; }
  }
  print(int_to_string(till_25));
  print(int_to_string(till_50));
  print(int_to_string(till_75));
  print(int_to_string(till_100));
}
