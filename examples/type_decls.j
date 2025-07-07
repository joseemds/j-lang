type Type1 = Int;
type Type2 = Type1;
type Array = [Type1];

type Type3 = {
  x, y, z : Type1;
  b : Bool;
};

type Type4 = enum {"e1", "e2", "e3"};

val a : Type2 = 5;


type Pessoa = {
  nome: String;
  idade: Int;
};

type Pessoas = [Pessoa];

val carlos : Pessoas = [Pessoa {nome: "Carlos Breno", idade: 18}, Pessoa {nome: "Carlos Eduardo", idade: 23}, Pessoa {nome: "Roberto Carlos", idade: 99}];

carlos[0].nome = "";

print(carlos[1].nome);
