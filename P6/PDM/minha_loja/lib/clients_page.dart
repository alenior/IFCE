import 'package:flutter/material.dart';

class ClientesPage extends StatelessWidget {
  const ClientesPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Clientes Cadastrados'),
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
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
                    Icon(Icons.person, size: 50),
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
              // Botão para cadastrar novo cliente
              ElevatedButton(
                onPressed: () {
                  _showCadastroDialog(context);
                },
                child: Text('Cadastrar Novo Cliente'),
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
