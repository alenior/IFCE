class Pessoa {
  late String _nome; // atributo privado
  late int _idade; // atributo privado
  String get nome => _nome; // Método assessor
  set nome(String nome) => _nome = nome; // Método Modificador
  int get idade => _idade; // Método assessor
  set idade(int idade) => _idade = idade;
}

void main() {
  Pessoa pessoa = Pessoa();
  pessoa.nome = "João"; // chama o método modificador "set nome"
  pessoa.idade = 30; // chama o método modificador "set idade"
  print(pessoa.nome); // chama o método assessor "get nome"
  print(pessoa.idade); // chama o método assessor "get idade"
}
