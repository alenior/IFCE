import 'product.dart';
import 'client.dart';

class Order {
  final String orderNumber;
  final Client client;
  final Map<Product, int> items;

  Order({
    required this.orderNumber,
    required this.client,
    required this.items,
  });

  double get totalPrice {
    return items.entries
        .map((entry) => entry.key.price * entry.value)
        .reduce((value, element) => value + element);
  }

  get id => null;

  get clientName => null;

  get totalValue => null;
}
