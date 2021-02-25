#include "Projetil.hpp"

Projetil::Projetil(float dano, float tempo_de_recarga, sf::Vector2f alcance,
	Personagem* dono) :
	ArtefatoAtaque(dano, tempo_de_recarga, alcance, dono, true) {
    ativar();
}

Projetil::~Projetil() {
}

Projetil::Projetil(const Projetil& copia)
:   ArtefatoAtaque(copia._dano, copia._tempo_de_recarga, 
    copia._area, copia._dono, copia._projetil) {
    _id_tipo_objeto = copia.getIdTipoObjeto();
    _movendo = copia._movendo;
    setPosicao(copia.getPosicao());
    setVelocidade(copia.getVelocidade());
}

void Projetil::setAnimador(std::string caminho, int colunas, float tempo_por_quadro) {
	_movendo.setConfig(caminho, colunas, tempo_por_quadro);
}

void Projetil::mover() {
    para_direita = (getVelocidade().x > 0);
	_movendo.atualiza(para_direita, _corpo);
	EntidadeColidivel::mover();
}

bool Projetil::isDescartavel() const {
    return _descartavel;
}

void Projetil::setDescartavel(bool descartavel) {
    _descartavel = descartavel;
}

std::ofstream& Projetil::operator >> (std::ofstream& out) {
    out << getIdTipoObjeto()     << ' '
    << getPosicao()              << ' '
    << getVelocidade()           << ' ';
    return out;
}

std::ifstream& Projetil::operator << (std::ifstream& in) {
    sf::Vector2f posicao;
    sf::Vector2f velocidade;
    in >> posicao
    >> velocidade;
    setPosicao (posicao);
    setVelocidade (velocidade);
    return in;
} 
