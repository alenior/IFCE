import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import './providers/products_provider.dart';
import './providers/clients_provider.dart';
import './providers/orders_provider.dart';
import './screens/products_screen.dart';
import './screens/clients_screen.dart';
import './screens/orders_screen.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => ProductsProvider()),
        ChangeNotifierProvider(create: (_) => ClientsProvider()),
        ChangeNotifierProvider(create: (_) => OrdersProvider()),
      ],
      child: MaterialApp(
        title: 'Meu Catálogo',
        theme: ThemeData(primarySwatch: Colors.blue),
        home: ProductsScreen(),
        routes: {
          '/clients': (ctx) => ClientsScreen(),
          '/orders': (ctx) => OrdersScreen(),
        },
      ),
    );
  }
}
