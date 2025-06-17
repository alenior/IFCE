import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'firebase_options.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  runApp(const MonitoramentoArApp());
}

class MonitoramentoArApp extends StatelessWidget {
  const MonitoramentoArApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Qualidade do Ar',
      theme: ThemeData(primarySwatch: Colors.blue),
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
  final DatabaseReference _dbRef = FirebaseDatabase.instance.ref("leituras");
  int? pm1_0;
  int? pm2_5;
  int? pm10;

  @override
  void initState() {
    super.initState();
    _dbRef.onValue.listen((DatabaseEvent event) {
      final data = event.snapshot.value as Map<dynamic, dynamic>?;
      setState(() {
        pm1_0 = data?["pm1_0"];
        pm2_5 = data?["pm2_5"];
        pm10 = data?["pm10"];
      });
    });
  }

  Widget _buildLeitura(String titulo, int? valor) {
    return Card(
      margin: const EdgeInsets.all(12),
      child: ListTile(
        title: Text(titulo, style: const TextStyle(fontSize: 18)),
        trailing: Text(
          valor != null ? "$valor µg/m³" : "--",
          style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Monitoramento do Ar')),
      body: ListView(
        children: [
          _buildLeitura("PM1.0", pm1_0),
          _buildLeitura("PM2.5", pm2_5),
          _buildLeitura("PM10", pm10),
        ],
      ),
    );
  }
}
