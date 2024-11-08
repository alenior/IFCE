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
  print('var and na base 10 = ' + and.toRadixString(10).padLeft(4,'0'));
  // > 00010000

  var or = 52 | 17;
  print('52 em binário = ' + 52.toRadixString(2).padLeft(8, '0'));
  // > 00110100
  print('17 em binario = ' + 17.toRadixString(2).padLeft(8, '0'));
  // > 00010001
  print('52 ord 17 = ' + or.toRadixString(2).padLeft(8, '0'));
  // > 00010000
  print('var or na base 10 = ' + and.toRadixString(10).padLeft(4,'0'));
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
}
