import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:intl/intl.dart';

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
      theme: ThemeData(primarySwatch: Colors.teal),
      home: const LeituraSensorPage(),
      debugShowCheckedModeBanner: false,
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
  double? latitude;
  double? longitude;
  double? altitude;
  String? datetime;

  @override
  void initState() {
    super.initState();
    _dbRef.onValue.listen((DatabaseEvent event) {
      final data = event.snapshot.value as Map<dynamic, dynamic>?;
      if (data != null) {
        setState(() {
          pm1_0 = data['pm1_0'];
          pm2_5 = data['pm2_5'];
          pm10 = data['pm10'];
          latitude = (data['latitude'] ?? 0.0).toDouble();
          longitude = (data['longitude'] ?? 0.0).toDouble();
          altitude = (data['altitude'] ?? 0.0).toDouble();
          datetime = data['datetime_utc'];
        });
      }
    });
  }

  Widget _buildLeitura(String titulo, String valor, [IconData? icon]) {
    return Card(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
      child: ListTile(
        leading: icon != null ? Icon(icon) : null,
        title: Text(titulo),
        trailing: Text(valor, style: const TextStyle(fontWeight: FontWeight.bold)),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final hasData = pm1_0 != null || pm2_5 != null || pm10 != null;
    final hasFix = latitude != null && longitude != null && latitude != 0.0 && longitude != 0.0;
    final DateTime? parsedTime = DateTime.tryParse(datetime ?? "");

    return Scaffold(
      appBar: AppBar(title: const Text('Monitoramento do Ar')),
      body: hasData
          ? ListView(
              children: [
                _buildLeitura("PM1.0", pm1_0 != null ? "$pm1_0 µg/m³" : "--", Icons.blur_on),
                _buildLeitura("PM2.5", pm2_5 != null ? "$pm2_5 µg/m³" : "--", Icons.blur_circular),
                _buildLeitura("PM10", pm10 != null ? "$pm10 µg/m³" : "--", Icons.blur_linear),
                _buildLeitura("Altitude", altitude != null ? "${altitude!.toStringAsFixed(2)} m" : "--", Icons.terrain),
                _buildLeitura(
                  "Data/Hora UTC",
                  parsedTime != null
                      ? DateFormat("dd/MM/yyyy HH:mm:ss").format(parsedTime.toLocal())
                      : "Sem fix GPS",
                  Icons.access_time,
                ),
                const SizedBox(height: 16),
                hasFix
                    ? SizedBox(
                        height: 300,
                        child: GoogleMap(
                          initialCameraPosition: CameraPosition(
                            target: LatLng(latitude!, longitude!),
                            zoom: 15,
                          ),
                          markers: {
                            Marker(
                              markerId: const MarkerId("local"),
                              position: LatLng(latitude!, longitude!),
                              infoWindow: const InfoWindow(title: "Local da medição"),
                            )
                          },
                        ),
                      )
                    : const Padding(
                        padding: EdgeInsets.all(20.0),
                        child: Center(child: Text("GPS sem sinal ou fix – localização indisponível.")),
                      ),
              ],
            )
          : const Center(child: CircularProgressIndicator()),
    );
  }
}
