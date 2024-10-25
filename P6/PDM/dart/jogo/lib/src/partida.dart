const pedra = 'Pedra';
const papel = 'Papel';
const tesoura = 'Tesoura';
const lagarto = 'Lagarto';
const spock = 'Spock';

const opcoes = [pedra, papel, tesoura, lagarto, spock];

class Partida {
  final regras = <String, Map<String, String>>{};
  void criarRegra(String vencedor,
      {required String acao, required String perdedor}) {
    if (!regras.containsKey(vencedor)) {
      regras[vencedor] = {};
    }
    regras[vencedor]![acao] = perdedor;
  }
}
