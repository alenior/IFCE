import 'package:flutter/material.dart';
import '../models/product.dart';

class ProductItem extends StatelessWidget {
  final Product product;

  const ProductItem({super.key, required this.product});

  @override
  Widget build(BuildContext context) {
    return ListTile(
      leading: Image.network(product.imageUrl),
      title: Text(product.name),
      subtitle: Text('R\$ ${product.price.toStringAsFixed(2)}'),
      onTap: () {
        // Lógica para adicionar ao pedido
      },
    );
  }
}
