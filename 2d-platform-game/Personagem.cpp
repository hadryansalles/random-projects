#include "Personagem.hpp"

Personagem::Personagem(float vida, bool do_bem, bool para_direita, bool vivo)
:   _do_bem (do_bem)
,   _vivo (vivo)
,   _vida (vida)
,   _para_direita (para_direita)
{
} 
Personagem::~Personagem() {
	_vida = 0;
} 

void Personagem::diminuirVidaEm(float dano) {
	_vida -= dano;
    if (_receber_dano.getStatus() == sf::SoundSource::Status::Stopped) {
        _receber_dano.play();
    }
}
float Personagem::getVida() const {
	return _vida;
}
void Personagem::setVida( float vida) {
    _vida = vida;
}
bool Personagem::getDoBem() {
	return _do_bem;
}
void Personagem::setDoBem(bool do_bem) {
	_do_bem = do_bem;
}
void Personagem::matar() {
	_vivo = false;
	_vida = 0;
	_morte.setConfig("sprites/morte-sprite.png"
		, 5, 0.05f);
	_morte.atualiza(_para_direita, _corpo);
}
bool Personagem::morreu() {
	if (!estaVivo()) {
		_morte.atualiza(_para_direita, _corpo);
		if (_morte.terminou()) {
			return true;
		}
	}
	return false;
}

bool Personagem::estaVivo() {
    return (_vivo);
}

bool Personagem::estaParaDireita() {
    return _para_direita;
}

void Personagem::setEstaParaDireita(bool para_direita) {
    _para_direita = para_direita;
}

void Personagem::setSomDano(std::string caminho) {
    if (!_b_receber_dano.loadFromFile(caminho)) {
        std::cerr << "Personagem::setSomDano: "
        << "nao foi possivel carregar o audio "
        << caminho << std::endl;
    }
    _receber_dano.setBuffer(_b_receber_dano);
}

ArtefatoAtaque* Personagem::getArtefatoAtaque() const {
    return NULL;
} 
