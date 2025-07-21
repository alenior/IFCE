import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'firebase_options.dart';
import 'historico_page.dart';
import 'widgets/info_card.dart';
import 'services/reverse_geocoding.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Monitoramento da Qualidade do Ar',
      theme: ThemeData(useMaterial3: true, colorSchemeSeed: Colors.deepPurple),
      debugShowCheckedModeBanner: false,
      home: const MonitoramentoPage(),
    );
  }
}

class MonitoramentoPage extends StatefulWidget {
  const MonitoramentoPage({super.key});

  @override
  State<MonitoramentoPage> createState() => _MonitoramentoPageState();
}

class _MonitoramentoPageState extends State<MonitoramentoPage> {
  final DatabaseReference _dbRef = FirebaseDatabase.instance.ref();
  Map<String, dynamic> dados = {};
  String nomeLocal = '';
  GoogleMapController? _mapController;
  LatLng? _localizacao;

  @override
  void initState() {
    super.initState();
    _dbRef.child('dados').orderByKey().limitToLast(1).onChildAdded.listen((event) {
      final novo = Map<String, dynamic>.from(event.snapshot.value as Map);
      setState(() {
        dados = novo;
        _localizacao = LatLng(
          double.tryParse(novo['latitude'].toString()) ?? 0,
          double.tryParse(novo['longitude'].toString()) ?? 0,
        );
        _buscarNomeLocal();
      });
    });
  }

  Future<void> _buscarNomeLocal() async {
    if (_localizacao == null) return;
    final nome = await GeocodingService.getPlaceName(_localizacao!.latitude, _localizacao!.longitude);
    setState(() => nomeLocal = nome ?? 'Não identificado');

  }

  Color _corPM(double valor, double limite) {
    if (valor <= limite) return Colors.green;
    if (valor <= limite * 1.5) return Colors.orange;
    return Colors.red;
  }

  Widget _buildMapa() {
    if (_localizacao == null) return const SizedBox(height: 200, child: Center(child: CircularProgressIndicator()));

    final marcador = Marker(
      markerId: const MarkerId('local'),
      position: _localizacao!,
    );

    return SizedBox(
      height: 300,
      child: GoogleMap(
        initialCameraPosition: CameraPosition(target: _localizacao!, zoom: 17),
        markers: {marcador},
        onMapCreated: (controller) => _mapController = controller,
        myLocationButtonEnabled: false,
        zoomControlsEnabled: true,
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final pm1 = double.tryParse(dados['pm1_0']?.toString() ?? '') ?? 0;
    final pm2_5 = double.tryParse(dados['pm2_5']?.toString() ?? '') ?? 0;
    final pm10 = double.tryParse(dados['pm10']?.toString() ?? '') ?? 0;
    final altitude = double.tryParse(dados['altitude']?.toString() ?? '') ?? 0;
    final dataUtc = dados['utc'] ?? '';

    final pmCards = [
      InfoCard(
        titulo: 'PM1.0',
        valor: '$pm1 µg/m³ (Recomendável: 0 µg/m³)',
        cor: Colors.white,
        icone: Icons.blur_on,
        textoCor: Colors.black,
      ),
      InfoCard(
        titulo: 'PM2.5',
        valor: '$pm2_5 µg/m³ (Recomendável: < 15 µg/m³)',
        cor: _corPM(pm2_5, 15),
        icone: Icons.blur_circular,
      ),
      InfoCard(
        titulo: 'PM10',
        valor: '$pm10 µg/m³ (Recomendável: < 50 µg/m³)',
        cor: _corPM(pm10, 50),
        icone: Icons.grid_view_rounded,
      ),
      InfoCard(
        titulo: 'Altitude',
        valor: '${altitude.toStringAsFixed(2)} m',
        cor: Colors.blueGrey,
        icone: Icons.terrain,
      ),
      InfoCard(
        titulo: 'Data/Hora UTC',
        valor: dataUtc,
        cor: Colors.teal,
        icone: Icons.access_time,
      ),
      InfoCard(
        titulo: 'Local Aproximado',
        valor: nomeLocal,
        cor: Colors.purple,
        icone: Icons.place,
      ),
    ];

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
          ),
        ],
      ),
      body: LayoutBuilder(
        builder: (context, constraints) {
          return SingleChildScrollView(
            padding: const EdgeInsets.all(12),
            child: Column(
              children: [
                ...pmCards,
                const SizedBox(height: 16),
                _buildMapa(),
              ],
            ),
          );
        },
      ),
    );
  }
}
