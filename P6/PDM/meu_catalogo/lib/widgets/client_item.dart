import 'package:flutter/material.dart';
import '../models/client.dart';

class ClientItem extends StatelessWidget {
  final Client client;

  ClientItem({required this.client});

  @override
  Widget build(BuildContext context) {
    return ListTile(
      title: Text(client.name),
      subtitle: Text(client.email),
      trailing: Text(client.phone),
      onTap: () {
        // Ação ao selecionar um cliente
      },
    );
  }
}