import 'package:flutter/material.dart';
import 'clients_page.dart';

class PedidosPage extends StatelessWidget {
  const PedidosPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Pedidos Cadastrados'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
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
        ],
      ),
      body: SingleChildScrollView(
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Container(
                margin: const EdgeInsets.all(10), // Margem externa
                child: Text.rich(
                  textAlign: TextAlign.center,
                  TextSpan(
                    text: 'PEDIDOS',
                    style: TextStyle(fontWeight: FontWeight.bold, fontSize: 25),
                  ),
                ),
              ),
              // Botão para adicionar pedido
              ElevatedButton(
                onPressed: () {
                  // Lógica para adicionar novo pedido
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => NovoPedidoPage()),
                  );
                },
                child: Text('Adicionar Novo Pedido'),
              ),
              // Lista de pedidos (mock ou banco de dados)
              PedidoItem(
                imagem:
                    'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/esp32.webp',
                cliente: 'Homer Simpson',
                produto: 'Esp32',
                quantidade: 2,
                preco: 100.00,
                status: 'Pendente',
              ),
              PedidoItem(
                imagem:
                    'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/fonte5v.webp',
                cliente: 'Marge Simpson',
                produto: 'Fonte 5v',
                quantidade: 1,
                preco: 15.00,
                status: 'Concluído',
              ),
              PedidoItem(
                imagem:
                    'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/protoboard830.webp',
                cliente: 'Bart Simpson',
                produto: 'Protoboard',
                quantidade: 3,
                preco: 30.00,
                status: 'Em andamento',
              ),
              PedidoItem(
                imagem:
                    'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/ledVermelho.webp',
                cliente: 'Lisa Simpson',
                produto: 'Led vermelho',
                quantidade: 5,
                preco: 1.00,
                status: 'Pendente',
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class PedidoItem extends StatelessWidget {
  final String imagem;
  final String produto;
  final String cliente;
  final int quantidade;
  final double preco;
  final String status;

  const PedidoItem({
    super.key,
    required this.imagem,
    required this.produto,
    required this.cliente,
    required this.quantidade,
    required this.preco,
    required this.status,
  });

  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.all(16), // Margem externa
      padding: const EdgeInsets.all(8), // Espaçamento interno
      decoration: BoxDecoration(
        color: Colors.white, // Cor de fundo
        border: Border.all(color: Colors.black, width: 2), // Borda
        borderRadius: BorderRadius.circular(10), // Bordas arredondadas
      ),
      child: Row(
        children: [
          Image.network(imagem, width: 150, height: 150),
          const SizedBox(width: 16),
          Expanded(
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Text(
                  'Cliente: $cliente',
                  style: TextStyle(fontWeight: FontWeight.bold),
                ),
                Text('Produto: $produto'),
                Text('Quantidade: $quantidade'),
                Text('Status: $status'),
              ],
            ),
          ),
        ],
      ),
    );
  }
}

class NovoPedidoPage extends StatelessWidget {
  const NovoPedidoPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Novo Pedido'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
      ),
      body: Center(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            children: [
              TextField(decoration: InputDecoration(labelText: 'Produto')),
              TextField(decoration: InputDecoration(labelText: 'Cliente')),
              TextField(
                decoration: InputDecoration(labelText: 'Quantidade'),
                keyboardType: TextInputType.number,
              ),
              DropdownButton<String>(
                hint: Text('Status'),
                items:
                    ['Pendente', 'Em andamento', 'Concluído']
                        .map(
                          (status) => DropdownMenuItem<String>(
                            value: status,
                            child: Text(status),
                          ),
                        )
                        .toList(),
                onChanged: (value) {
                  // Lógica para selecionar status
                },
              ),
              ElevatedButton(
                onPressed: () {
                  // Lógica para salvar o pedido
                  Navigator.pop(context); // Volta à página de pedidos
                },
                child: Text('Salvar Pedido'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
