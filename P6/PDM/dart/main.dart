library logger;
import 'dart:io';

enum Cores { verde, vermelho, azul }

const resetarCor = '\x1b[m';
const ansiCores = {
  Cores.vermelho: '\x1b[31m',
  Cores.verde: '\x1b[32m',
  Cores.azul: '\x1b[36m',
};

void info(Object object) => log(Cores.verde, '[INFO] $object');
void warning(Object object) => log(Cores.azul, '[WARNING] $object');
void error(Object object) => log(Cores.vermelho, '[ERROR] $object');

void log(Cores cor, Object object) {
  stdout.writeln('${ansiCores[cor]}$object$resetarCor');
}
