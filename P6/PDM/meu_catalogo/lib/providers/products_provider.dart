import 'package:flutter/material.dart';
import '../models/product.dart';

class ProductsProvider with ChangeNotifier {
  final List<Product> _products = [
    Product(
      code: 'P001',
      imageUrl: 'build/flutter_assets/images/produto1.jpeg',
      name: 'Produto 1',
      description: 'Descrição do Produto 1',
      price: 50.0,
    ),
    // Adicione mais produtos...
  ];

  List<Product> get products => [..._products];
}
