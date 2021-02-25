#include "Fantasma.hpp"
#include "Fase.hpp"

const float Fantasma::TEMPO_FANTASMA (0.2f);

Fantasma::Fantasma(sf::Vector2f posicao)
:   Inimigo(2.0f, false, false)
,   caveira_de_fogo(1, 2,
    sf::Vector2f(10.0f,10.0f), this)
,   _tempo_ataque (0.0f)
{
    setIdTipoObjeto(Codigos::FANTASMA);
	setPosicao(posicao);
	carregarAnimacoes();
	setArea(sf::Vector2f(35, 35));
	caveira_de_fogo.setVelocidade(sf::Vector2f(6.0f, 3.0f));
	caveira_de_fogo.setPosicao(posicao);
    caveira_de_fogo.setIdTipoObjeto(Codigos::BOLA_FOGO);
	surgindo.atualiza(true, _corpo, TEMPO_FANTASMA);
	setVelocidade(sf::Vector2f(0, 0));\
    setEstatico(true);
	_esta_nascendo = true;
	esta_atacando = false;
}

Fantasma::~Fantasma(){
}

void Fantasma::mover(){
	if (estaVivo() && !esta_atacando && !_esta_nascendo) {
		if (getJogadorMaisProximo()->getPosicao().x > getPosicao().x) {
			setEstaParaDireita(false);
		}
		else {
			setEstaParaDireita(true);
		}
		parado.atualiza(estaParaDireita(), _corpo);
	}
	else if (_esta_nascendo) {
		if (surgindo.terminou()) {
			_esta_nascendo = false;
		}
		else {
			surgindo.atualiza(estaParaDireita(), _corpo);
		}
	}
	EntidadeColidivel::mover();
}

void Fantasma::carregarAnimacoes(){
	soltando_caveira.setConfig("sprites/fantasma-cast.png", 4,
		TEMPO_FANTASMA);
	parado.setConfig("sprites/fantasma.png", 7,
		TEMPO_FANTASMA);
	surgindo.setConfig("sprites/fantasma-spawn.png", 6,
		TEMPO_FANTASMA);
	_morte.setConfig("sprites/fantasma-death.png", 7,
		TEMPO_FANTASMA);

	caveira_de_fogo.setAnimador("sprites/caveira-fogo_att.png", 8, 0.15f);
}

Projetil* Fantasma::atacar() {
	if (!estaVivo() || (getJogador1() == NULL && getJogador2() == NULL)) {
        return NULL;
    }
	Projetil* atk = NULL;
    _tempo_ataque += Fase::getCronometro();
	if (estaVivo() && !_esta_nascendo && !esta_atacando) {
		if (caveira_de_fogo.recarregou()) {
			esta_atacando = true;
			soltando_caveira.reiniciar();
		}
	}
	else if (esta_atacando) {
		if (soltando_caveira.executou(3) && _tempo_ataque > 1.0f) {
            _tempo_ataque = 0;
			atk = new Projetil (caveira_de_fogo);
            atk->setPosicao(getPosicao());
            atk->ativar();
            sf::Vector2f vel;
			vel.x = caveira_de_fogo.getVelocidade().x * direcaoNormalizada(getJogadorMaisProximo()).x;
			vel.y = caveira_de_fogo.getVelocidade().y * direcaoNormalizada(getJogadorMaisProximo()).y;
			atk->setVelocidade(vel);
			caveira_de_fogo.setTempoUltimoArtefatoAtaque(0);
		}
		if (!soltando_caveira.terminou()) {
			soltando_caveira.atualiza(estaParaDireita(), _corpo);
		}
		else {
			soltando_caveira.zerar();
			parado.atualiza(estaParaDireita(), _corpo);
			esta_atacando = false;
		}
	}
	return atk;
} 
