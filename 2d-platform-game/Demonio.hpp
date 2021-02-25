#pragma once

#include "Std.hpp"
#include "Inimigo.hpp"
#include "ArtefatoAtaque.hpp"
#include "Projetil.hpp"

class Demonio : public Inimigo {
private:
	ArtefatoAtaque* _fogo;

	Animador atacando;
	Animador voando;
	Animador tomando_dano;
	
	bool esta_atacando;
	float posicao_x_onde_ira_atacar;
	bool indo_para_a_posicao_de_ataque;
	float mover_ate;
	bool esta_tomando_dano;

    sf::Vector2f _pos_alvo;
    sf::Vector2f _direcao_alvo;
    bool _atacando = false;
    bool _indo_atacar = true;
    bool _jogador_alvo = false;
        
	static const float Velocidade;

    void carregarAnimacoes();

public:
	Demonio(sf::Vector2f pos);
	virtual ~Demonio();

	ArtefatoAtaque* getArtefatoAtaque() const;
	Projetil* atacar();
	void mover();
	void diminuirVidaEm(float dano);
};
