#pragma once

#include "ArtefatoAtaque.hpp"
#include "Inimigo.hpp"

class Ladrao : public Inimigo {

private:
	ArtefatoAtaque* _espada;

	Animador correndo;
	Animador atacando;
	Animador tomando_dano;

	bool esta_atacando;
	bool esta_tomando_dano;

	void carregarAnimacoes();
public:
	Ladrao(sf::Vector2f pos);
	~Ladrao();

	ArtefatoAtaque* getArtefatoAtaque();
	Projetil* atacar();
	void mover();
	void diminuirVidaEm(float dano);
};
