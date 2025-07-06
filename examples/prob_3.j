func matrix_sum(lines, cols : Int, matrix_1, matrix_2 : [[Int]]) : [[Int]] {
  val result : [[Int]];
  for(val i : Int = 0; i < lines; i = i + 1) {
    val line : [Int];
    for(val j : Int = 0; j < cols; j = j + 1) { 
      append(line, matrix_1[i][j] + matrix_2[i][j]);
    }
    append(result, line);
  }
  return result;
}

func matrix_mul(lines_1, cols_1, lines_2, cols_2 : Int, matrix_1, matrix_2 : [[Int]]) : [[Int]] {
  val result : [[Int]];
  for(val k : Int = 0; k < lines_1; k = k + 1) {
    val line : [Int];
    for(val i : Int = 0; i < cols_1; i = i + 1) {
      val value : Int = 0;
      for(val j : Int = 0; j < lines_2; j = j + 1) {
        value = value + (matrix_1[k][j] * matrix_2[j][i]);
      }
      append(line, value);
    }
    append(result, line);
  }
}

func matrix_print(lines, cols : Int, matrix : [[Int]]) : Void {
  for(val i : Int = 0; i < lines; i = i + 1) {
    for(val j : Int = 0; j < cols; j = j + 1) {
      print(int_to_str(matrix[i][j]) + " ");
    }
    print("\n");
  }
}

func read_matrix(lines, cols : Int) : [[Int]] {
  val matrix : [[Int]];
  for(val i : Int = 0; i < lines; i = i + 1) {
    val line : [Int];
    for(val j : Int = 0; j < cols; j = j + 1) {
      append(line, str_to_int(input()));
    }
    append(matrix, line);
  }
  return matrix;
}

func main() : Void {
  val inputs_array : [Int] = split(input(), " ");
  -- val lines_1, cols_1, lines_2, cols_2 : Int;

  val matrix_1, matrix_2 : [[Int]] = read_matrix(lines_1, cols_1), read_matrix(lines_2, cols_2);
  
  if (lines_1 != lines_2 || cols_1 != cols_2) {
    print("Matrizes de ordems diferentes não podem ser somadas.");
  } else {
    print("Soma: " + matrix_print(matrix_sum(m1, m2)));
  }

  if (cols_1 != lines_2){
    print("Não é possível multiplicar essas matrizes.");
  } else {
    print("Produto: " + matrix_print(matrix_mul(m1, m2)));
  }

}