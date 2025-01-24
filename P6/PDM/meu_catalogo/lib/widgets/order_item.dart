import 'package:flutter/material.dart';
import '../models/order.dart';

class OrderItem extends StatelessWidget {
  final Order order;

  OrderItem({required this.order});

  @override
  Widget build(BuildContext context) {
    return Card(
      margin: EdgeInsets.symmetric(vertical: 8, horizontal: 16),
      child: ListTile(
        title: Text('Pedido #${order.id}'),
        subtitle: Text('Cliente: ${order.clientName}'),
        trailing: Text('R\$ ${order.totalValue.toStringAsFixed(2)}'),
        onTap: () {
          // Ação ao selecionar um pedido
        },
      ),
    );
  }
}