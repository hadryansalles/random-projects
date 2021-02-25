#pragma once
#include "Esqueleto.hpp"

class EsqueletoRoupa : public Esqueleto {

private:
    // Atributos do esqueleto
    static const float vida;
    static const float valor_morte;
    static const float dano;
    static const float tempo_recarga;
    static const sf::Vector2f area;
    static const sf::Vector2f velocidade;

	//carrega texturas do personagem
	void carregarAnimacoes();
public:
	EsqueletoRoupa(sf::Vector2f posicao = sf::Vector2f(0.0f, 0.0f),
        bool do_bem = false, bool para_direita = true);
	~EsqueletoRoupa();

    virtual void mover();
};
