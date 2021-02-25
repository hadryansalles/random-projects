#include "Demonio.hpp"

const float Demonio::Velocidade(1.5f);

Demonio::Demonio(sf::Vector2f pos)
:	Inimigo(10, false, false, 1000) {
	_fogo = new ArtefatoAtaque(20, 2.0f, sf::Vector2f(90, 50), (Personagem*)this);
    setIdTipoObjeto( Codigos::DEMONIO );
	_fogo->desativar();
	esta_atacando = false;
	esta_tomando_dano = false;
	setEstaParaDireita(false);
	setPosicao(pos);
	carregarAnimacoes();
	setArea(sf::Vector2f(50, 100));
	voando.atualiza(estaParaDireita(), _corpo, 0.2f);
	posicao_x_onde_ira_atacar = 0;
	indo_para_a_posicao_de_ataque = false;
} 
Demonio::~Demonio() {

} 
ArtefatoAtaque* Demonio::getArtefatoAtaque() const {
	return _fogo;
} 
void Demonio::carregarAnimacoes() {
	voando.setConfig("sprites/demonio-voando.png", 6,
		0.2f);
	atacando.setConfig("sprites/demonio-atacando.png", 11,
		0.2f);
	tomando_dano.setConfig("sprites/demonio-dano.png", 10,
		0.05f);
} 
void Demonio::diminuirVidaEm(float dano) {
	_vida -= dano;

	esta_tomando_dano = true;
	tomando_dano.reiniciar();
	tomando_dano.atualiza(estaParaDireita(), _corpo);
	if (esta_atacando) {
		esta_atacando = false;
	}
} 

Projetil* Demonio::atacar() {
	if (estaParaDireita()) {
		_fogo->setPosicao(getPosicao().x - _fogo->getArea().x, getPosicao().y + _fogo->getArea().y);
	}
	else {
		_fogo->setPosicao(getPosicao().x + _fogo->getArea().x, getPosicao().y + _fogo->getArea().y);
	}
	if (estaVivo() && !esta_tomando_dano) {
		if (!esta_atacando) {
			if (indo_para_a_posicao_de_ataque) {
				if (abs(posicao_x_onde_ira_atacar - getPosicao().x) < _fogo->getArea().x) { // chegou na area de ataque
					indo_para_a_posicao_de_ataque = false;
					esta_atacando = true;
					atacando.reiniciar();
				}
			}
			else {
				if (_fogo->recarregou()) {
					posicao_x_onde_ira_atacar = getJogadorMaisProximo()->getPosicao().x;
					indo_para_a_posicao_de_ataque = true;
				}
			}
		}
		else if (esta_atacando) {
			atacando.atualiza(estaParaDireita(), _corpo);
			if (atacando.executou(8)) {
				_fogo->ativar();
				_fogo->setTempoUltimoArtefatoAtaque(0);
			}
			if (atacando.terminou()) {
				atacando.zerar();
				_fogo->desativar();
				esta_atacando = false;
				if (getPosicao().x < 384 / 2) {
					mover_ate = 400;
				}
				else {
					mover_ate = 70;
				}
			}
		}
	}
	return NULL;
} /*}}}}*/

void Demonio::mover() {
	if (!estaVivo() || (getJogador1() == NULL && getJogador2() == NULL)) {
		return;
	}
	_velocidade.y = 0;
	setVelocidade(getVelocidade().x, _velocidade.y);
	if (!esta_atacando && estaVivo() && !esta_tomando_dano) {
		if (indo_para_a_posicao_de_ataque) {
			if (posicao_x_onde_ira_atacar > getPosicao().x) {
				setVelocidade(Velocidade, _velocidade.y);
				setEstaParaDireita(false);
			}
			else {
				setVelocidade(-Velocidade, _velocidade.y);
				setEstaParaDireita(true);
			}
		}
		else {
			if (mover_ate > getPosicao().x) {
				setVelocidade(Velocidade, _velocidade.y);
				setEstaParaDireita(false);
			}
			else {
				setVelocidade(-Velocidade, _velocidade.y);
				setEstaParaDireita(true);
			}
		}
		voando.atualiza(estaParaDireita(), _corpo);
		EntidadeColidivel::mover();
	}
	else if (esta_tomando_dano) {
		tomando_dano.atualiza(estaParaDireita(), _corpo);
		if (tomando_dano.terminou()) {
			esta_tomando_dano = false;
			tomando_dano.zerar();
		}
	}
}
