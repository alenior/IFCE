import 'package:flutter/material.dart';

class InfoCard extends StatelessWidget {
  final String titulo;
  final String valor;
  final Color cor;
  final IconData icone;
  final Color textoCor;

  const InfoCard({
    super.key,
    required this.titulo,
    required this.valor,
    required this.cor,
    required this.icone,
    this.textoCor = Colors.white,
  });

  @override
  Widget build(BuildContext context) {
    return Card(
      color: cor,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(16)),
      margin: const EdgeInsets.symmetric(vertical: 6),
      elevation: 4,
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Row(
          children: [
            Icon(icone, size: 30, color: textoCor),
            const SizedBox(width: 16),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(titulo, style: TextStyle(fontSize: 16, fontWeight: FontWeight.bold, color: textoCor)),
                  const SizedBox(height: 4),
                  Text(valor, style: TextStyle(fontSize: 14, color: textoCor)),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }
}
