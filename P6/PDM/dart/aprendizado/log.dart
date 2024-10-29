library logger;

import 'dart:io';
part 'printer.dart';

enum _Cores { verde, vermelho, azul }

const _resetarCor = '\x1b[m';
const _ansiCores = {
  _Cores.vermelho: '\x1b[31m',
  _Cores.verde: '\x1b[32m',
  _Cores.azul: '\x1b[36m',
};

void info(Object object) => _log(_Cores.verde, '[INFO] $object');
void warning(Object object) => _log(_Cores.azul, '[WARNING] $object');
void error(Object object) => _log(_Cores.vermelho, '[ERROR] $object');

void _log(_Cores cor, Object object) {
  stdout.writeln('${_ansiCores[cor]}$object$_resetarCor');
}

enum Nivel { info, warning, error }

class Logger {
  const Logger({
    required this.nivel,
    this.printer = const Printer(),
  });
  final Printer printer;
  final Nivel nivel;
  void info(Object object) {
    if (_habilitado(Nivel.info)) printer._log(_Cores.verde, '[INFO] $object');
  }

  void warning(Object object) {
    if (_habilitado(Nivel.warning)) printer._log(_Cores.azul, '[WARNING] $object');
  }

  void error(Object object) {
    if (_habilitado(Nivel.error)) printer._log(_Cores.vermelho, '[ERROR] $object');
  }

  bool _habilitado(Nivel nivelHabilitado) =>
      nivelHabilitado.index >= nivel.index;
}

void main() {
  const logger = Logger(
    nivel: Nivel.error,
    printer: Printer(inicio: 'Customizado: {', fim: '}'),
  );
  logger.error('Este é um erro');
  logger.warning('Este é um warning');
  logger.info('Esta é uma info');
}

class Printer {
  const Printer({this.inicio = '', this.fim = ''});
  final String inicio;
  final String fim;
  void _log(_Cores cor, Object object) {
    stdout.writeln(
      '${_ansiCores[cor]}'
      '$inicio$object$fim'
      '$_resetarCor',
    );
  }
}