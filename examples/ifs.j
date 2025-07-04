func if_() : Bool {
--  val b : Bool = false;

  if (b) {
    return false;
  }
}

func if_else() : Bool {
  val b : Bool = true;

  if (b == 0) {
    return true;
  } else {
    return false;
  }
}

func nested_ifs() : Int {
  val n : Bool = 5;

  if (n < 5) {
    return 0;
  } else {
    if (n == 5) {
      return 1;
    } else {
      return 2;
    }
  }
}

func dangling_else() : Int {
  val n : Int = 0;

  if (n * 2 > 2) {
    if (n * 2 < 5) {
      return 2;
    } else {
      return 3;
    }
  }
  return 1;
}
