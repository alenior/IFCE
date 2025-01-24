void main() {
  print('Hello, Dart!');

  print(50 + 2); // > 52
  print('50' + '2'); // > 502
  print(50 - 7); // > 43
  print(-(-50)); // > 50
  print(25 * 2); // > 50
  print(25 / 2); // > 12.5
  print(25 ~/ 2); // > 12
  print(25 % 2); // > 1

  print(50 == 50); // > true
  print(50 != 50); // > false
  print(50 > 50); // > false
  print(50 >= 50); // > true
  print(50 < 50); // > false
  print(50 <= 50); // > true

  print(50 == 50 && 50 <= 10); // > false
  print((50 == 50 && 50 <= 10) || 52 != 52); // > false
  print(!(50 == 50 && 50 <= 10) || 52 != 20); // > true

  var and = 52 & 17;
  print('52 em binário = ' + 52.toRadixString(2).padLeft(8, '0'));
  // > 00110100
  print('17 em binario = ' + 17.toRadixString(2).padLeft(8, '0'));
  // > 00010001
  print('52 and 17 = ' + and.toRadixString(2).padLeft(8, '0'));
  // > 00010000
  print('var and na base 10 = ' + and.toRadixString(10).padLeft(4, '0'));
  // > 00010000

  var or = 52 | 17;
  print('52 em binário = ' + 52.toRadixString(2).padLeft(8, '0'));
  // > 00110100
  print('17 em binario = ' + 17.toRadixString(2).padLeft(8, '0'));
  // > 00010001
  print('52 ord 17 = ' + or.toRadixString(2).padLeft(8, '0'));
  // > 00010000
  print('var or na base 10 = ' + and.toRadixString(10).padLeft(4, '0'));
  // > 00010000

  var xor = 52 ^ 17;
  print('52 em binário = ' + 52.toRadixString(2).padLeft(8, '0'));
  // > 00110100
  print('17 em binario = ' + 17.toRadixString(2).padLeft(8, '0'));
  // > 00010001
  print('52 xor 17 = ' + xor.toRadixString(2).padLeft(8, '0'));
  // > 00100101
  print('52 xor 17 na base 10 =' + xor.toRadixString(10).padLeft(4, '0'));
  // > 0037

  var not = ~52;
  print(52.toRadixString(2).padLeft(8, '0')); // > 00110100
  print(not); // > -53
  print(not.toRadixString(2)); // > -110101

  var shift = 52 >> 1;
  print(52.toRadixString(2).padLeft(8, '0')); // > 00110100
  print(shift); // > 26
  print(shift.toRadixString(2).padLeft(8, '0')); // > 00011010

  var shift0 = 52 << 1;
  print(52.toRadixString(2).padLeft(8, '0')); // > 00110100
  print(shift0); // > 104
  print(shift0.toRadixString(2).padLeft(8, '0')); // > 01101000

  var shift1 = -42 >>> 1;
  print((-42).toRadixString(2).padLeft(8, '0')); // > 0-101010
  print(shift1); // > 9223372036854775787
  print(shift1.toRadixString(2));
  // > 111111111111111111111111111111111111111111111111111111111101011

  var a = 0;
  var b = 1 + ++a; // 1 + 1
  print(a); // > 1
  print(b); // > 2
  var c = 0;
  var d = 1 + --c; // 1 + -1
  print(c); // > -1
  print(d); // > 0

  // Converte o tipo num para int
  // em seguida acessa a propriedade bitlenght
  num a0 = 42;
  print((a0 as int).bitLength); // > 6

  // Testa o tipo da variável num
  num a1 = 42;
  print(a1 is int); // > true
  print(a1 is! String); // > true

  final frase = StringBuffer();
  frase.write('Operação ');
  frase.write('em ');
  frase.write('cascade.');

  final frase0 = StringBuffer()
    ..write('Operação ')
    ..write('em ')
    ..write('cascade.');
  print(frase0); // > Operação em cascade.

  int x = 22;
  print(x % 2 == 0 ? 'par' : 'ímpar'); // > par

  final map = {
    'vogais': 'a,e,i,o,u',
    'consoantes': 'b,c,d,...',
  };
  final vogais = ['a', 'e', 'i', 'o', 'u'];
  print(vogais[0]); // > a
  print(vogais[4]); // > u
  print(map['vogais']); // > a,e,i,o,u

//admite que a variável quantidade pode ser nula
  int? quantidade = null;
  quantidade ??= 10; // se quantidade for nula, recebe o valor
  print(quantidade); // > 10

  int? resposta = null;
  resposta ??= 42;
  print(resposta); // > 42

  StringBuffer? frase1 = null;
  frase..write('Operação ')
    ..write('em ')
    ..write('cascade.');
  print(frase1); // > null

  List<String>? vogais0;
  print(vogais0?[1]); // > null

  int idade = 25;
  double salario = 5000.00;
  num desconto = 5.5;

  String nome = "João";

  var buffer = new StringBuffer();
  // adiciona várias cadeias de caracteres
  buffer.write('Olá');
  buffer.write(' ');
  buffer.write('Mundo!');
  print(buffer.toString()); // saida: Olá Mundo!

Pessoa pessoa = Pessoa();
  pessoa.nome = "João"; // chama o método modificador "set nome"
  pessoa.idade = 30; // chama o método modificador "set idade"
  print(pessoa.nome); // chama o método assessor "get nome"
  print(pessoa.idade); // chama o método assessor "get idade"
}

class Pessoa {
  late String _nome; // atributo privado
  late int _idade; // atributo privado
  String get nome => _nome; // Método assessor
  set nome(String nome) => _nome = nome; // Método Modificador
  int get idade => _idade; // Método assessor
  set idade(int idade) => _idade = idade;
}