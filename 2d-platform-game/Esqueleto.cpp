#include "Esqueleto.hpp"

Esqueleto::Esqueleto(float vida, sf::Vector2f posicao, bool do_bem,
bool para_direita, float valor_morte) 
:   Inimigo(vida, do_bem, para_direita, valor_morte) {
    _esta_nascendo = true;
	setPosicao(posicao.x, posicao.y);
    setSomDano("sounds/crash.wav");
}

Esqueleto::~Esqueleto() {
    proprio_corpo->desativar();
    proprio_corpo->descartar();
}
void Esqueleto::diminuirVidaEm(float dano) {
    Personagem::diminuirVidaEm(dano);
}
Projetil* Esqueleto::atacar() {
    // morre se fizer um ataque	
	if (proprio_corpo->getAtivado()) {
		atualizarArtefatoAtaqueCorpo();
	}
	else {
		_vida = 0;
	}
	return NULL;
}
void Esqueleto::atualizarArtefatoAtaqueCorpo() {
	proprio_corpo->setPosicao(getPosicao().x, getPosicao().y);
}
ArtefatoAtaque* Esqueleto::getArtefatoAtaque() {
	return proprio_corpo;
}
