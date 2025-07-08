func main () : Void {
  val till_25, till_50, till_75, till_100 : Int = 0, 0, 0, 0;
  while(true) {
    val input : Int = str_to_int(input());
    if (input < 0) { break; }
    if (input < 26) { till_25 = till_25 + 1; continue; }
    if (input < 51)  { till_50 = till_50 + 1; continue; }
    if (input < 76) { till_75 = till_75 + 1; continue; }
    if (input < 101) {till_100 = till_100 + 1; }
  }
  print("[0, 25]: " + int_to_string(till_25));
  print("[26, 50]: " + int_to_string(till_50));
  print("[51, 75]: " + int_to_string(till_75));
  print("[76, 100]: " + int_to_string(till_100));
}
