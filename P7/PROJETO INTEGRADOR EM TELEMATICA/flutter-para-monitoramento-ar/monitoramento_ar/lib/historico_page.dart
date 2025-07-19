import 'package:flutter/material.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:intl/intl.dart';
import 'geocoding_service.dart';

class HistoricoPage extends StatefulWidget {
  const HistoricoPage({super.key});

  @override
  State<HistoricoPage> createState() => _HistoricoPageState();
}

class _HistoricoPageState extends State<HistoricoPage> {
  final _database = FirebaseDatabase.instance.ref('leituras');
  List<Map<dynamic, dynamic>> _registros = [];
  DateTime? _inicio;
  DateTime? _fim;
  String _periodoTexto = "";

  @override
  void initState() {
    super.initState();
    _carregarHistorico();
  }

  Future<void> _carregarHistorico() async {
    final snapshot = await _database.get();
    final Map<dynamic, dynamic>? valores = snapshot.value as Map<dynamic, dynamic>?;

    if (valores != null) {
      List<Map<dynamic, dynamic>> registros = valores.entries.map((e) {
        final item = Map<String, dynamic>.from(e.value);
        item['id'] = e.key;
        return item;
      }).toList();

      registros.sort((a, b) => (b['datetime_utc'] ?? '').compareTo(a['datetime_utc'] ?? ''));

      DateTime? primeiro = _converterUTCParaBrasilia(registros.last['datetime_utc']);
      DateTime? ultimo = _converterUTCParaBrasilia(registros.first['datetime_utc']);

      setState(() {
        _registros = registros;
        _inicio = null;
        _fim = null;
        _periodoTexto = "${DateFormat('dd/MM/yyyy').format(primeiro!)} a ${DateFormat('dd/MM/yyyy').format(ultimo!)}";
      });
    }
  }

  DateTime? _converterUTCParaBrasilia(String? utcString) {
    if (utcString == null) return null;
    try {
      final utcDate = DateTime.parse(utcString);
      return utcDate.toLocal();
    } catch (_) {
      return null;
    }
  }

  Future<void> _selecionarPeriodo() async {
    final hoje = DateTime.now();
    final DateTimeRange? selecionado = await showDateRangePicker(
      context: context,
      firstDate: DateTime(2020),
      lastDate: hoje,
    );

    if (selecionado != null) {
      setState(() {
        _inicio = selecionado.start;
        _fim = selecionado.end;
        _periodoTexto = "${DateFormat('dd/MM/yyyy').format(_inicio!)} a ${DateFormat('dd/MM/yyyy').format(_fim!)}";
      });
    }
  }

  void _limparFiltro() {
    setState(() {
      _inicio = null;
      _fim = null;
      _carregarHistorico();
    });
  }

  bool _dentroDoPeriodo(DateTime data) {
    if (_inicio == null || _fim == null) return true;
    return !data.isBefore(_inicio!) && !data.isAfter(_fim!);
  }

  @override
  Widget build(BuildContext context) {
    final registrosFiltrados = _registros.where((item) {
      final data = _converterUTCParaBrasilia(item['datetime_utc']);
      return data != null && _dentroDoPeriodo(data);
    }).toList();

    return Scaffold(
      appBar: AppBar(
        title: const Text("Histórico de Leituras"),
        actions: [
          if (_periodoTexto.isNotEmpty)
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8),
              child: Center(child: Text(_periodoTexto, style: const TextStyle(fontSize: 14))),
            ),
          IconButton(
            icon: const Icon(Icons.calendar_today),
            onPressed: _selecionarPeriodo,
          ),
          IconButton(
            icon: const Icon(Icons.clear),
            onPressed: _limparFiltro,
          ),
        ],
      ),
      body: ListView.builder(
        itemCount: registrosFiltrados.length,
        itemBuilder: (context, index) {
          final item = registrosFiltrados[index];
          return Card(
            child: ExpansionTile(
              title: Text("${item['datetime_utc']} - ${item['pm2_5']} µg/m³"),
              subtitle: Text("Qualidade: ${item['qualidade'] ?? '---'}"),
              children: [
                FutureBuilder<String?>(
                  future: GeocodingService.getPlaceName(
                    item['latitude']?.toDouble() ?? 0.0,
                    item['longitude']?.toDouble() ?? 0.0,
                  ),
                  builder: (context, snapshot) {
                    final local = snapshot.data ?? "--";
                    return Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text("PM1.0: ${item['pm1_0']} µg/m³"),
                          Text("PM10: ${item['pm10']} µg/m³"),
                          Text("Latitude: ${item['latitude']}"),
                          Text("Longitude: ${item['longitude']}"),
                          Text("Altitude: ${item['altitude']} m"),
                          Text("Local aproximado: $local"),
                          Text("UTC: ${item['datetime_utc']}")
                        ],
                      ),
                    );
                  },
                )
              ],
            ),
          );
        },
      ),
    );
  }
}