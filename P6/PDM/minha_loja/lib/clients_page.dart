import 'package:flutter/material.dart';
import 'pedidos_page.dart';

class ClientesPage extends StatelessWidget {
  const ClientesPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Clientes Cadastrados'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        actions: [
          Container(
            padding: const EdgeInsets.only(right: 50), // Afasta da margem direita
            child: IconButton(
              icon: Icon(Icons.shopping_cart),  // Ícone de carrinho de compras
              onPressed: () {
                // Navega para a tela de pedidos
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => PedidosPage()),  // Redireciona para a página Pedidos
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
                    text: 'CLIENTES',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontSize: 25,
                    ),
                  ),
                ),
              ),
              // Botão para cadastrar novo cliente
              ElevatedButton(
                onPressed: () {
                  _showCadastroDialog(context);
                },
                child: Text('Cadastrar Novo Cliente'),
              ),
              // Aqui você pode listar clientes cadastrados (mock ou banco de dados)
              Container(
                margin: const EdgeInsets.all(16), // Margem externa
                padding: const EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(10), // Bordas arredondadas
                ),
                child: Row(
                  children: [
                    Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/homerSimpson.webp',
                      width: 150,
                      height: 150,
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Nome: Homer Simpson',
                            style: TextStyle(fontWeight: FontWeight.bold),
                          ),
                          Text('CPF: 123.456.789-00'),
                          Text('Telefone: (11) 91234-5678'),
                          Text('Email: homer@email.com'),
                          Text('Endereço: Evergreen Terrace, 742'),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
              Container(
                margin: const EdgeInsets.all(16), // Margem externa
                padding: const EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(10), // Bordas arredondadas
                ),
                child: Row(
                  children: [
                    Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/margeSimpson.webp',
                      width: 150,
                      height: 150,
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Nome: Marge Simpson',
                            style: TextStyle(fontWeight: FontWeight.bold),
                          ),
                          Text('CPF: 123.456.789-11'),
                          Text('Telefone: (11) 91234-5678'),
                          Text('Email: marge@email.com'),
                          Text('Endereço: Evergreen Terrace, 742'),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
              Container(
                margin: const EdgeInsets.all(16), // Margem externa
                padding: const EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(10), // Bordas arredondadas
                ),
                child: Row(
                  children: [
                    Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/bartSimpson.webp',
                      width: 150,
                      height: 150,
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Nome: Bart Simpson',
                            style: TextStyle(fontWeight: FontWeight.bold),
                          ),
                          Text('CPF: 123.456.789-22'),
                          Text('Telefone: (11) 91234-5678'),
                          Text('Email: bart@email.com'),
                          Text('Endereço: Evergreen Terrace, 742'),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
              Container(
                margin: const EdgeInsets.all(16), // Margem externa
                padding: const EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(10), // Bordas arredondadas
                ),
                child: Row(
                  children: [
                    Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/lisaSimpson.webp',
                      width: 150,
                      height: 150,
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Nome: Lisa Simpson',
                            style: TextStyle(fontWeight: FontWeight.bold),
                          ),
                          Text('CPF: 123.456.789-33'),
                          Text('Telefone: (11) 91234-5678'),
                          Text('Email: lisa@email.com'),
                          Text('Endereço: Evergreen Terrace, 742'),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
              Container(
                margin: const EdgeInsets.all(16), // Margem externa
                padding: const EdgeInsets.all(8), // Espaçamento interno
                decoration: BoxDecoration(
                  color: Colors.white, // Cor de fundo
                  border: Border.all(color: Colors.black, width: 2), // Borda
                  borderRadius: BorderRadius.circular(10), // Bordas arredondadas
                ),
                child: Row(
                  children: [
                    Image.network(
                      'https://raw.githubusercontent.com/alenior/IFCE/refs/heads/main/P6/PDM/minha_loja/lib/media/megSimpson.webp',
                      width: 150,
                      height: 150,
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Nome: Meg Simpson',
                            style: TextStyle(fontWeight: FontWeight.bold),
                          ),
                          Text('CPF: 123.456.789-44'),
                          Text('Telefone: (11) 91234-5678'),
                          Text('Email: meg@email.com'),
                          Text('Endereço: Evergreen Terrace, 742'),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  void _showCadastroDialog(BuildContext context) {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        String nome = '';
        String cpf = '';
        String telefone = '';
        String email = '';
        String endereco = '';

        return AlertDialog(
          title: const Text('Cadastrar Novo Cliente'),
          content: SingleChildScrollView(
            child: Column(
              children: [
                TextField(
                  decoration: InputDecoration(labelText: 'Nome'),
                  onChanged: (value) => nome = value,
                ),
                TextField(
                  decoration: InputDecoration(labelText: 'CPF'),
                  onChanged: (value) => cpf = value,
                ),
                TextField(
                  decoration: InputDecoration(labelText: 'Telefone'),
                  onChanged: (value) => telefone = value,
                ),
                TextField(
                  decoration: InputDecoration(labelText: 'Email'),
                  onChanged: (value) => email = value,
                ),
                TextField(
                  decoration: InputDecoration(labelText: 'Endereço'),
                  onChanged: (value) => endereco = value,
                ),
              ],
            ),
          ),
          actions: [
            TextButton(
              onPressed: () {
                // Lógica para salvar o cliente (mock por enquanto)
                print('Cliente Cadastrado: $nome, $cpf, $telefone, $email, $endereco');
                Navigator.of(context).pop();
              },
              child: const Text('Salvar'),
            ),
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: const Text('Cancelar'),
            ),
          ],
        );
      },
    );
  }
}
