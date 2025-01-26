import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Minha Loja',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.red),
      ),
      home: const MyHomePage(title: 'Minha Loja'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: SingleChildScrollView(
        child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Row(
              children: [
                Image.network(
                  'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/esp32.webp',
                  width: 150,
                  height: 150,
                ),
                Text('Esp32'),
                Text('Microcontrolador poderoso com acesso à internet via wifi.'),
              ],
            ),
            Row(
              children: [
                Image.network(
                  'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/protoboard830.webp',
                  width: 150,
                  height: 150,
                ),
                Text('Protoboard'),
                Text('Matriz de 830 furos para conexão de condutores sem necessitar soldar.'),
              ],
            ),
            Row(
              children: [
                Image.network(
              'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/fonte5v.webp',
                  width: 150,
                  height: 150,
                ),
                Text('Fonte 5v'),
                Text('Fonte de tensão 5v e 3v3 por pinos e USB.'),
              ],
            ),
          ],
        ),
      ),
      ),
    );
  }
}
