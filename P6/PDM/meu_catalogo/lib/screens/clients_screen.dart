import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../providers/clients_provider.dart';
import '../widgets/client_item.dart';

class ClientsScreen extends StatelessWidget {
  const ClientsScreen({super.key});

  @override
  Widget build(BuildContext context) {
    final clientsData = Provider.of<ClientsProvider>(context);
    final clients = clientsData.clients;

    return Scaffold(
      appBar: AppBar(title: Text('Clientes')),
      body: ListView.builder(
        itemCount: clients.length,
        itemBuilder: (ctx, i) => ClientItem(client: clients[i]),
      ),
    );
  }
}