import 'package:flutter/material.dart';
import 'clients_page.dart';
import 'pedidos_page.dart';

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
        actions: [
          Container(
            padding: const EdgeInsets.only(
              right: 50,
            ), // Afasta da margem direita
            child: IconButton(
              icon: Icon(Icons.people),
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => ClientesPage()),
                );
              },
            ),
          ),
          Container(
            padding: const EdgeInsets.only(
              right: 50,
            ), // Afasta da margem direita
            child: IconButton(
              icon: Icon(Icons.shopping_cart), // Ícone de carrinho de compras
              onPressed: () {
                // Navega para a tela de pedidos
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (context) => PedidosPage(),
                  ), // Redireciona para a página Pedidos
                );
              },
            ),
          ),
        ],
      ),
      body: SingleChildScrollView(
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Container(
                margin: EdgeInsets.all(10), // Margem externa
                child: Text.rich(
                  textAlign: TextAlign.center,
                  TextSpan(
                    text: 'PRODUTOS',
                    style: TextStyle(fontWeight: FontWeight.bold, fontSize: 25),
                  ),
                ),
              ),
              Container(
                margin: EdgeInsets.all(16), // Margem externa
                padding: EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(
                    10,
                  ), // Bordas arredondadas
                ),
                child: Wrap(
                  children: [
                    Column(
                      children: [
                        Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/esp32.webp',
                      width: 150,
                      height: 150,
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Esp32',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 500,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text:
                              'Microcontrolador poderoso com acesso à internet via wifi.',
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'R\$ 50,00',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                      ],
                    )
                  ],
                ),
              ),
              Container(
                margin: EdgeInsets.all(16), // Margem externa
                padding: EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(
                    10,
                  ), // Bordas arredondadas
                ),
                child: Wrap(
                  children: [
                    Column(
                      children: [
                        Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/protoboard830.webp',
                      width: 150,
                      height: 150,
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Protoboard',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 500,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text:
                              'Matriz de 830 furos para conexão de condutores sem necessitar soldar.',
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'R\$ 10,00',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                      ],
                    )
                  ],
                ),
              ),
              Container(
                margin: EdgeInsets.all(16), // Margem externa
                padding: EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(
                    10,
                  ), // Bordas arredondadas
                ),
                child: Wrap(
                  children: [
                    Column(
                      children: [
                        Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/fonte5v.webp',
                      width: 150,
                      height: 150,
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Fonte 5v',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 500,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Fonte de tensão 5v e 3v3 por pinos e USB.',
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'R\$ 15,00',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                      ],
                    )
                  ],
                ),
              ),
              Container(
                margin: EdgeInsets.all(16), // Margem externa
                padding: EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(
                    10,
                  ), // Bordas arredondadas
                ),
                child: Wrap(
                  children: [
                    Column(
                      children: [
                        Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/ledVermelho.webp',
                      width: 150,
                      height: 150,
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Led vermelho',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 500,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text:
                              'Led vermelho difuso de 5mm ideal para prototipagem.',
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'R\$ 0,20',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                      ],
                    )
                  ],
                ),
              ),
              Container(
                margin: EdgeInsets.all(16), // Margem externa
                padding: EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(
                    10,
                  ), // Bordas arredondadas
                ),
                child: Wrap(
                  children: [
                    Column(
                      children: [
                        Image.network(
                      'https://github.com/alenior/IFCE/blob/main/P6/PDM/minha_loja/lib/media/ledVerde.jpeg?raw=true',
                      width: 150,
                      height: 150,
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'Led verde',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 500,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text:
                              'Led verde difuso de 5mm ideal para prototipagem.',
                        ),
                      ),
                    ),
                    SizedBox(
                      width: 150,
                      child: Text.rich(
                        textAlign: TextAlign.center,
                        TextSpan(
                          text: 'R\$ 0,20',
                          style: TextStyle(fontWeight: FontWeight.bold),
                        ),
                      ),
                    ),
                      ],
                    )
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
