#pragma once

#include "Std.hpp"
#include "Personagem.hpp"
#include "Jogador.hpp"
#include "Projetil.hpp"

class Inimigo : public Personagem {
private:
	static const Jogador* p_jog_1;
	static const Jogador* p_jog_2;
    const float _valor_morte;

protected:
	bool _esta_nascendo;
    sf::Vector2f direcaoNormalizada(const Jogador* jogador) const;
sf::Vector2f direcaoNormalizada(sf::Vector2f posicao) const;
	const Jogador* getJogadorMaisProximo() const;
    
public:
	Inimigo(float vida = 1.0f, bool do_bem = true, bool para_direita = true, float valor_morte = 100.0f);
	virtual ~Inimigo();

	virtual void mover() = 0;
	virtual Projetil* atacar() = 0;
    static void setJogador1(const Jogador* jog);
    static void setJogador2(const Jogador* jog);
    static const Jogador* getJogador1();
    static const Jogador* getJogador2();
    float getValorMorte() const;
	virtual void diminuirVidaEm(float dano);
    std::ofstream& operator >> (std::ofstream& out);
    std::ifstream& operator << (std::ifstream& in);
};
