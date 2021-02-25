#include "Ladrao.hpp"

Ladrao::Ladrao(sf::Vector2f pos)
:	Inimigo(5, false, false)
{
	_espada = new ArtefatoAtaque(2, 1.0f, sf::Vector2f(32, 5), (Personagem*)this);
    setIdTipoObjeto( Codigos::LADRAO );
	_espada->desativar();
	esta_atacando = false;
	esta_tomando_dano = false;
	setEstaParaDireita(false);
	setPosicao(pos);
	carregarAnimacoes();
	setArea(sf::Vector2f(32, 41));
	correndo.atualiza(estaParaDireita(), _corpo, 0.2f);
} 

Ladrao::~Ladrao() {
}

ArtefatoAtaque* Ladrao::getArtefatoAtaque() {
	return _espada;
}

void Ladrao::carregarAnimacoes() {
	correndo.setConfig("sprites/ladrao-correndo.png", 6,
		0.2f);
	atacando.setConfig("sprites/ladrao-atacando.png", 5,
		0.15f);
	tomando_dano.setConfig("sprites/ladrao-dano.png", 1, 
		0.2f);
}

Projetil* Ladrao::atacar() {

	if (estaParaDireita()) {
		_espada->setPosicao(getPosicao().x + getArea().x, getPosicao().y - 5.0f);
	}
	else {
		_espada->setPosicao(getPosicao().x - getArea().x, getPosicao().y - 5.0f);
	}

	if (estaVivo() && !esta_atacando && _espada->recarregou() && !esta_tomando_dano) {
		if (distanciaEuclidiana(getJogadorMaisProximo()) - 10 < _espada->getArea().x) {
			esta_atacando = true;
			atacando.reiniciar();
		}
	}
	else if (esta_atacando) {

		atacando.atualiza(estaParaDireita(), _corpo);
		if (atacando.executou(3)) {
			_espada->ativar();
			_espada->setTempoUltimoArtefatoAtaque(0);
		}
		if (atacando.terminou()) {
			atacando.zerar();
			_espada->desativar();
			esta_atacando = false;
		}
	}
	else if (esta_tomando_dano) {
		tomando_dano.atualiza(estaParaDireita(), _corpo);
		if (tomando_dano.terminou()) {
			esta_tomando_dano = false;
			tomando_dano.zerar();
		}
	}
	return NULL;
}

void Ladrao::mover() {
	if (!estaVivo() || (getJogador1() == NULL && getJogador2() == NULL)) {
        return;
    }
	if (estaVivo() && !esta_atacando && !esta_tomando_dano) {
		if (getPosicao().x < getJogadorMaisProximo()->getPosicao().x) {
			setEstaParaDireita( true );
			setVelocidade(1.25, getVelocidade().y);
		}
		else {
			setEstaParaDireita(false);
			setVelocidade( - 1.25, getVelocidade().y);
		}
		correndo.atualiza(estaParaDireita(), _corpo);
		EntidadeColidivel::mover();
	}
}

void Ladrao::diminuirVidaEm(float dano) {
	_vida -= dano;
	esta_tomando_dano = true;
	esta_atacando = false;
	atacando.zerar();
	_espada->desativar();
	tomando_dano.reiniciar();
	tomando_dano.atualiza(estaParaDireita(), _corpo);
}
