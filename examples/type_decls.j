type Type1 = Int;
type Type2 = Type1;
type Type3 = {
  val x, y, z : Type1;
  val b : Bool = false;
}
type Type4 = enum {"e0", "e1", "e2"};

val a : Type2 = 5;

type Type5 = enum {`template literal example number ${a}`};