import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:intl/intl.dart';

class HistoricoPage extends StatefulWidget {
  const HistoricoPage({super.key});

  @override
  State<HistoricoPage> createState() => _HistoricoPageState();
}

class _HistoricoPageState extends State<HistoricoPage> {
  final DatabaseReference _historicoRef = FirebaseDatabase.instance.ref("historico");

  Map<String, dynamic> historico = {};
  String? selectedKey;
  DateTime? selectedDate;
  String qualidadeFiltro = 'Todos';

  @override
  void initState() {
    super.initState();
    _historicoRef.onValue.listen((event) {
      final data = event.snapshot.value as Map<dynamic, dynamic>?;
      if (data != null) {
        final Map<String, dynamic> temp = {};
        data.forEach((key, value) {
          if (value is Map) {
            temp[key.toString()] = Map<String, dynamic>.from(value);
          }
        });
        setState(() => historico = temp);
      }
    });
  }

  List<MapEntry<String, dynamic>> get filteredList {
    final entries = historico.entries.toList();
    entries.sort((a, b) {
      final dtA = DateTime.tryParse(a.value['datetime_utc'] ?? '') ?? DateTime(0);
      final dtB = DateTime.tryParse(b.value['datetime_utc'] ?? '') ?? DateTime(0);
      return dtB.compareTo(dtA); // ordem decrescente
    });

    return entries.where((entry) {
      final item = entry.value;
      final dtString = item['datetime_utc'] ?? '';
      final dt = DateTime.tryParse(dtString);

      if (selectedDate != null && dt != null) {
        if (dt.year != selectedDate!.year ||
            dt.month != selectedDate!.month ||
            dt.day != selectedDate!.day) {
          return false;
        }
      }

      if (qualidadeFiltro != 'Todos') {
        final q = item['qualidade'] ?? 'Desconhecida';
        if (q != qualidadeFiltro) return false;
      }

      return true;
    }).toList();
  }

  void _selecionarData() async {
    final DateTime now = DateTime.now();
    final DateTime? picked = await showDatePicker(
      context: context,
      initialDate: selectedDate ?? now,
      firstDate: DateTime(2024),
      lastDate: now,
    );
    if (picked != null) setState(() => selectedDate = picked);
  }

  Color _corQualidade(String qualidade) {
    switch (qualidade) {
      case 'Bom':
        return Colors.green;
      case 'Moderado':
        return Colors.orange;
      case 'Ruim':
        return Colors.red;
      default:
        return Colors.grey;
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Histórico de Leituras"),
        actions: [
          IconButton(
            icon: const Icon(Icons.date_range),
            onPressed: _selecionarData,
            tooltip: 'Filtrar por data',
          ),
          DropdownButton<String>(
            value: qualidadeFiltro,
            onChanged: (value) => setState(() => qualidadeFiltro = value!),
            items: ['Todos', 'Bom', 'Moderado', 'Ruim']
                .map((e) => DropdownMenuItem(value: e, child: Text(e)))
                .toList(),
          ),
        ],
      ),
      body: ListView.builder(
        itemCount: filteredList.length,
        itemBuilder: (context, index) {
          final entry = filteredList[index];
          final item = entry.value;
          final key = entry.key;
          final qualidade = item['qualidade'] ?? 'Desconhecida';
          final cor = _corQualidade(qualidade);
          final dt = DateTime.tryParse(item['datetime_utc'] ?? '')?.toLocal();

          return Card(
            margin: const EdgeInsets.all(8),
            child: ExpansionTile(
              onExpansionChanged: (expanded) => setState(() => selectedKey = expanded ? key : null),
              initiallyExpanded: selectedKey == key,
              leading: Icon(Icons.circle, color: cor),
              title: Text("PM2.5: ${item['pm2_5']} µg/m³ - $qualidade"),
              subtitle: dt != null
                  ? Text(DateFormat("dd/MM/yyyy HH:mm:ss").format(dt))
                  : const Text("Data inválida"),
              children: [
                Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text("PM1.0: ${item['pm1_0']} µg/m³"),
                      Text("PM10: ${item['pm10']} µg/m³"),
                      Text("Latitude: ${item['latitude']}"),
                      Text("Longitude: ${item['longitude']}"),
                      Text("Altitude: ${item['altitude']} m"),
                      Text("UTC: ${item['datetime_utc']}"),
                    ],
                  ),
                ),
                if (selectedKey == key &&
                    item['latitude'] != null &&
                    item['longitude'] != null &&
                    item['latitude'] != 0.0 &&
                    item['longitude'] != 0.0)
                  SizedBox(
                    height: 200,
                    child: GoogleMap(
                      initialCameraPosition: CameraPosition(
                        target: LatLng(item['latitude'], item['longitude']),
                        zoom: 15,
                      ),
                      markers: {
                        Marker(
                          markerId: const MarkerId("mapaHistorico"),
                          position: LatLng(item['latitude'], item['longitude']),
                          infoWindow: const InfoWindow(title: "Local da medição"),
                        )
                      },
                    ),
                  ),
              ],
            ),
          );
        },
      ),
    );
  }
}
