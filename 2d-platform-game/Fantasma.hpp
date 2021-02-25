#pragma once

#include "Inimigo.hpp"
#include "Projetil.hpp"

class Fantasma : public Inimigo{

private:
	Projetil caveira_de_fogo;

	Animador soltando_caveira;
	Animador parado;
	Animador surgindo;
	Animador sumindo;
    float _tempo_ataque;
	bool esta_atacando;

	static const float TEMPO_FANTASMA;

public:
	Fantasma(sf::Vector2f posicao);
	~Fantasma();

	void carregarAnimacoes();
	Projetil* atacar();
	void mover();	
};
