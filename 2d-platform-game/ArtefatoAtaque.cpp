#include "ArtefatoAtaque.hpp"
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Fase.hpp"

int ArtefatoAtaque::quantidade ( 0 ) ;
ArtefatoAtaque::ArtefatoAtaque(float dano, float tempo_de_recarga, sf::Vector2f alcance,
Personagem* dono, bool projetil) 
:   _dano(dano)
,	_tempo_de_recarga(tempo_de_recarga)
,	_dono(dono)
,	_tempo_ultimo_ataque(tempo_de_recarga + 1)
,   _ativado (false)
,	_projetil(projetil) {
	this->setArea(alcance);
}

ArtefatoAtaque::~ArtefatoAtaque() {
	_dono = NULL;
}

bool ArtefatoAtaque::recarregou() {
	_tempo_ultimo_ataque += Fase::getCronometro();
	if (_tempo_ultimo_ataque >= _tempo_de_recarga) {
		return true;
	}
	return false;
}

void ArtefatoAtaque::atacar (Inimigo* inimigo) {
	inimigo->diminuirVidaEm(_dano);
    Jogador* dono = dynamic_cast<Jogador*> (_dono);
    if (dono) {
		if(inimigo->getVida() <= 0)
			dono->alterarPontuacao(inimigo->getValorMorte());
    }
    desativar();
}

void ArtefatoAtaque::atacar (Jogador* jogador) {
	jogador->diminuirVidaEm(_dano);
    desativar();
}

bool ArtefatoAtaque::ehProjetil() const {
	return _projetil;
}

Personagem* ArtefatoAtaque::getDono() const{
	return _dono;
}

void ArtefatoAtaque::desativar() {
    _ativado = false;
}

void ArtefatoAtaque::ativar() {
    _ativado = true;
    _tempo_ultimo_ataque = 0.0f;
}

bool ArtefatoAtaque::getAtivado() const {
 return (_ativado);
}

void ArtefatoAtaque::setTempoUltimoArtefatoAtaque(float tempo) {
    _tempo_ultimo_ataque = tempo;
}
