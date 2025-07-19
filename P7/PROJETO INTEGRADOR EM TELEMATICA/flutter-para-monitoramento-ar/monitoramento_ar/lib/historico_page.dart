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
  DateTime? selectedStartDate;
  DateTime? selectedEndDate;
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
      final dtA = DateTime.tryParse(a.value['datetime_utc'] ?? '')?.toLocal() ?? DateTime(0);
      final dtB = DateTime.tryParse(b.value['datetime_utc'] ?? '')?.toLocal() ?? DateTime(0);
      return dtB.compareTo(dtA); // ordem decrescente
    });

    return entries.where((entry) {
      final item = entry.value;
      final dtString = item['datetime_utc'] ?? '';
      final dt = DateTime.tryParse(dtString)?.toLocal();

      if (dt == null) return false;

      if (selectedStartDate != null && selectedEndDate != null) {
        final inicio = DateTime(selectedStartDate!.year, selectedStartDate!.month, selectedStartDate!.day);
        final fim = DateTime(selectedEndDate!.year, selectedEndDate!.month, selectedEndDate!.day, 23, 59, 59);
        if (dt.isBefore(inicio) || dt.isAfter(fim)) {
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

  void _selecionarPeriodo() async {
    final DateTime now = DateTime.now();

    final DateTimeRange? picked = await showDateRangePicker(
      context: context,
      initialDateRange: (selectedStartDate != null && selectedEndDate != null)
          ? DateTimeRange(start: selectedStartDate!, end: selectedEndDate!)
          : null,
      firstDate: DateTime(2024),
      lastDate: now,
      helpText: 'Selecionar intervalo de datas',
    );

    if (picked != null) {
      setState(() {
        selectedStartDate = picked.start;
        selectedEndDate = picked.end;
      });
    }
  }

  void _limparFiltros() {
    setState(() {
      selectedStartDate = null;
      selectedEndDate = null;
      qualidadeFiltro = 'Todos';
    });
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

  String _formatarDataPeriodo() {
    if (historico.isEmpty) return "Sem registros";

    List<DateTime> datas = historico.values.map((e) {
      final dt = DateTime.tryParse(e['datetime_utc'] ?? '')?.toLocal();
      return dt ?? DateTime(0);
    }).where((dt) => dt.year > 2020).toList();

    if (datas.isEmpty) return "Sem registros";

    datas.sort();
    final df = DateFormat('dd/MM/yyyy');

    if (selectedStartDate != null && selectedEndDate != null) {
      return "${df.format(selectedStartDate!)} a ${df.format(selectedEndDate!)}";
    } else {
      return "${df.format(datas.first)} a ${df.format(datas.last)}";
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Histórico de Leituras"),
        actions: [
          TextButton(
            onPressed: _limparFiltros,
            child: const Text("Limpar filtros", style: TextStyle(color: Color.fromARGB(255, 24, 19, 19))),
          ),
          IconButton(
            icon: const Icon(Icons.date_range),
            onPressed: _selecionarPeriodo,
            tooltip: 'Filtrar por período',
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
      body: Column(
        children: [
          Padding(
            padding: const EdgeInsets.symmetric(vertical: 8),
            child: Text(
              "Período: ${_formatarDataPeriodo()}",
              style: const TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
            ),
          ),
          Expanded(
            child: ListView.builder(
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
          ),
        ],
      ),
    );
  }
}
