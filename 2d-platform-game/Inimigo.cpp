#include "Inimigo.hpp"

const Jogador* Inimigo::p_jog_1 = NULL;
const Jogador* Inimigo::p_jog_2 = NULL;

Inimigo::Inimigo(float vida, bool do_bem, bool para_direita, float valor_morte) 
:   Personagem(vida, do_bem, para_direita)
,   _valor_morte ( valor_morte )
,   _esta_nascendo (false) {
} 
Inimigo::~Inimigo() {

} 

void Inimigo::setJogador1(const Jogador* jog) {
	p_jog_1 = jog;
}

void Inimigo::setJogador2(const Jogador* jog) {
	p_jog_2 = jog;
}

const Jogador* Inimigo::getJogador1() {
	return p_jog_1;
}

const Jogador* Inimigo::getJogador2() {
	return p_jog_2;
} 

const Jogador* Inimigo::getJogadorMaisProximo() const {
    if (getJogador1() != NULL && getJogador2() != NULL) {
        if (distanciaEuclidiana(getJogador1()) > distanciaEuclidiana(getJogador2())) {
            return getJogador2();
        }
        return getJogador1();
    }
    if (getJogador2() != NULL) {
        return getJogador2();
    }
    return getJogador1();
} 

sf::Vector2f Inimigo::direcaoNormalizada(const Jogador* jogador) const {
	sf::Vector2f direcao(0,0);
	if (jogador != NULL) {
        direcao.x = jogador->getPosicao().x - getPosicao().x;
        direcao.y = jogador->getPosicao().y - getPosicao().y;
        direcao.x /= (std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y));
        direcao.y /= (std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y));
	}
	return direcao;
}

sf::Vector2f Inimigo::direcaoNormalizada(sf::Vector2f posicao) const {
	sf::Vector2f direcao(0,0);
    if (posicao == getPosicao()) {
        return direcao;
    }
    direcao.x = posicao.x - getPosicao().x;
    direcao.y = posicao.y - getPosicao().y;
    direcao.x /= (std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y));
    direcao.y /= (std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y));
	return direcao;
}

float Inimigo::getValorMorte() const {
    return _valor_morte;
}

std::ofstream& Inimigo::operator >> (std::ofstream& out) {
    out << getIdTipoObjeto()     << ' '
    << (_esta_nascendo ? 1 : 0 ) << ' ' 
    << getVida()                 << ' '
    << getPosicao()              << ' '
    << getVelocidade()           << ' ';
    out << (estaParaDireita() ? 1 : 0);
    return out;
}

std::ifstream& Inimigo::operator << (std::ifstream& in) {
    float vida = 0;
    sf::Vector2f posicao;
    sf::Vector2f velocidade;
    int estaParaDireita = 0;
    int esta_nascendo = 0;

    in >> esta_nascendo
    >> vida
    >> posicao
    >> velocidade
    >> estaParaDireita;

    _esta_nascendo = (esta_nascendo == 0 ? false : true );
    _vida = float (vida);
    setPosicao (posicao);
    setVelocidade (velocidade);
    setEstaParaDireita(estaParaDireita == 0 ? false : true);
    return in;
} 

void Inimigo::diminuirVidaEm(float dano) {
	_vida -= dano;
}
