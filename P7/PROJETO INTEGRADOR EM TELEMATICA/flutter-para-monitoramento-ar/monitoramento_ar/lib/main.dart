import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'dart:async';
import 'historico_page.dart';
import 'geocoding_service.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Monitoramento do Ar',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const LeituraSensorPage(),
    );
  }
}

class LeituraSensorPage extends StatefulWidget {
  const LeituraSensorPage({super.key});

  @override
  State<LeituraSensorPage> createState() => _LeituraSensorPageState();
}

class _LeituraSensorPageState extends State<LeituraSensorPage> {
  final _historicoRef = FirebaseDatabase.instance.ref('leituras');

  int pm1_0 = 0;
  int pm2_5 = 0;
  int pm10 = 0;
  double latitude = 0;
  double longitude = 0;
  double altitude = 0;
  String datetime = "--";
  String? placeName;

  Color getStatusColor(String status) {
    switch (status) {
      case "Ruim":
        return Colors.red;
      case "Moderado":
        return Colors.yellow;
      default:
        return Colors.green;
    }
  }

  String classificarQualidadeComposta() {
    int score = 0;
    if (pm1_0 > 0) score++;
    if (pm1_0 > 10) score++;
    if (pm2_5 > 15) score++;
    if (pm2_5 > 50) score++;
    if (pm10 > 50) score++;
    if (pm10 > 100) score++;
    if (score <= 1) return "Bom";
    if (score <= 3) return "Moderado";
    return "Ruim";
  }

  late Timer _timer;
  bool _toggleColor = false;

  @override
  void initState() {
    super.initState();

    _historicoRef.orderByKey().limitToLast(1).onValue.listen((event) async {
      final children = event.snapshot.children;
      if (children.isNotEmpty) {
        final lastSnapshot = children.last;
        final data = lastSnapshot.value as Map<dynamic, dynamic>?;
        if (data != null) {
          final lat = (data['latitude'] ?? 0.0).toDouble();
          final lon = (data['longitude'] ?? 0.0).toDouble();
          String? place;
          if (lat != 0.0 && lon != 0.0) {
            place = await GeocodingService.getPlaceName(lat, lon);
          }

          setState(() {
            pm1_0 = data['pm1_0'];
            pm2_5 = data['pm2_5'];
            pm10 = data['pm10'];
            latitude = lat;
            longitude = lon;
            altitude = (data['altitude'] ?? 0.0).toDouble();
            datetime = data['datetime_utc'];
            placeName = place;
          });
        }
      }
    });

    _timer = Timer.periodic(const Duration(milliseconds: 500), (_) {
      setState(() => _toggleColor = !_toggleColor);
    });
  }

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
  }

  Widget _buildLeitura(String titulo, String valor, IconData icone, Color cor) {
    return Card(
      elevation: 4,
      color: cor,
      margin: const EdgeInsets.symmetric(vertical: 8, horizontal: 16),
      child: ListTile(
        leading: Icon(icone, size: 32, color: Colors.black),
        title: Text(titulo, style: const TextStyle(color: Colors.black, fontWeight: FontWeight.bold)),
        subtitle: Text(valor, style: const TextStyle(color: Colors.black)),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final status = classificarQualidadeComposta();
    final statusColor = getStatusColor(status);
    final dynamicColor = (status == "Ruim" && _toggleColor)
        ? Colors.red.shade900
        : (status == "Moderado" && _toggleColor)
            ? Colors.yellow.shade700
            : statusColor;

    return Scaffold(
      appBar: AppBar(
        title: const Text('Monitoramento do Ar'),
        actions: [
          IconButton(
            icon: const Icon(Icons.history),
            onPressed: () => Navigator.push(
              context,
              MaterialPageRoute(builder: (_) => const HistoricoPage()),
            ),
          )
        ],
      ),
      body: ListView(
        children: [
          _buildLeitura("PM1.0", "$pm1_0 µg/m³", Icons.blur_on, Colors.cyan.shade100),
          _buildLeitura("PM2.5", "$pm2_5 µg/m³", Icons.blur_circular, Colors.orange.shade100),
          _buildLeitura("PM10", "$pm10 µg/m³", Icons.blur_linear, Colors.purple.shade100),
          _buildLeitura("Qualidade do Ar", status, Icons.speed, dynamicColor),
          _buildLeitura("Latitude", "$latitude", Icons.my_location, Colors.grey.shade200),
          _buildLeitura("Longitude", "$longitude", Icons.location_on, Colors.grey.shade200),
          _buildLeitura("Altitude", "$altitude m", Icons.signal_cellular_alt, Colors.teal.shade100),
          _buildLeitura("Local aproximado", placeName ?? "--", Icons.place, Colors.purple.shade200),
          _buildLeitura("Data/Hora UTC", datetime, Icons.access_time, Colors.grey.shade300),
        ],
      ),
    );
  }
}