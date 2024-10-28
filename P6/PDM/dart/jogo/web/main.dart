import 'package:jogo/src/partida.dart';
import 'dart:html';
import 'package:jogo/src/resultado.dart';

late DivElement divOpcoes;
late DivElement divResultado;
bool jogando = true;
late Resultado resultado;

void main() {
  inicializarReferencias();
  final partida = configurarPartida();
  for (final opcao in opcoes) {
    divOpcoes.append(
      ImageButtonInputElement()
        ..className = 'opcao'
        ..src = 'images/$opcao.png'
        ..height = 120,
        ..onClick.listen(
          (MouseEvent e) {
            if (jogando) {
            resultado = partida.iniciar(humano: opcao);
            jogando = false;
            mostrarResultado(resultado);
            }
          },
        )
    ); 
    }
}

Partida configurarPartida() {
  return Partida()
    ..criarRegra(tesoura, acao: 'corta', perdedor: papel)
    ..criarRegra(tesoura, acao: 'decapita', perdedor: lagarto)
    ..criarRegra(papel, acao: 'cobre', perdedor: pedra)
    ..criarRegra(papel, acao: 'refuta', perdedor: spock)
    ..criarRegra(pedra, acao: 'esmaga', perdedor: lagarto)
    ..criarRegra(pedra, acao: 'quebra', perdedor: tesoura)
    ..criarRegra(lagarto, acao: 'envenena', perdedor: spock)
    ..criarRegra(lagarto, acao: 'come', perdedor: papel)
    ..criarRegra(spock, acao: 'esmaga', perdedor: tesoura)
    ..criarRegra(spock, acao: 'vaporiza', perdedor: pedra);
}

void inicializarReferencias() {
divOpcoes = querySelector('#opcoes') as DivElement;
divResultado = querySelector('#resultado') as DivElement;
}

void mostrarResultado(Resultado resultado) {
  String mensagem;
  String classeCss;

  switch (resultado.resultadoType) {
    case ResultadoType.empate:
      classeCss = 'empatou';
      mensagem = 'Empatou..';
      break;
    case ResultadoType.vitoria:
      classeCss = 'venceu';
      mensagem = 'Você ganhou :D';
      break;
    case ResultadoType.derrota:
      classeCss = 'perdeu';
      mensagem = 'Você perdeu :/';
      break;
  }

  adicionarEspaco();
    divResultado.append(SpanElement()..text = resultado.resumo);
    adicionarEspaco();
    divResultado.append(
      ButtonElement()
        ..text = 'Jogar novamente!'
        ..onClick.listen(jogarNovamente),
  );

  divResultado.append(
    SpanElement()
      ..className = classeCss
      ..text = mensagem,
  );
}