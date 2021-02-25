#pragma once
#include "Inimigo.hpp"
#include "ArtefatoAtaque.hpp"

class Esqueleto : public Inimigo {
protected:
	ArtefatoAtaque* proprio_corpo;
	Animador nascendo;
	Animador andando;
	void atualizarArtefatoAtaqueCorpo();

	//carrega texturas do personagem
    virtual	void carregarAnimacoes() = 0;

public:
	Esqueleto(float vida = 10.0f, sf::Vector2f posicao = sf::Vector2f(0.0f, 0.0f),
        bool do_bem = false, bool para_direita = true, float valor_morte = 100.0f);
	virtual ~Esqueleto();

    virtual void diminuirVidaEm(float dano);
	virtual void mover() = 0;
	virtual ArtefatoAtaque* getArtefatoAtaque();
	virtual Projetil* atacar();
};
