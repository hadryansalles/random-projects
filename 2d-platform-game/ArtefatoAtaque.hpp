#pragma once

#include "Std.hpp"
#include "EntidadeColidivel.hpp"

class Fase;
class Personagem;
class Inimigo;
class Jogador;

class ArtefatoAtaque : public EntidadeColidivel {
	
protected:
	float _dano;
	float _tempo_de_recarga;
	Personagem* _dono; //ataque conhece seu dono para nao causar dano a si mesmo;
	float _tempo_ultimo_ataque;
    bool _ativado;
	bool _projetil;
    static int quantidade;
public:
	ArtefatoAtaque(float dano, float tempo_de_recarga, sf::Vector2f alcance, Personagem* dono, bool projetil = false);
	virtual ~ArtefatoAtaque();
	//indica se ataque ja foi recarregado para poder ser usado novamente
	bool recarregou();
	Personagem* getDono() const;
	//instancia de ataque, objeto que seria inserido na lista de ataques
	bool ehProjetil() const;
    void atacar (Inimigo* inimigo);
    void atacar (Jogador* jogador);
    void desativar();
    void ativar();
    bool getAtivado() const;
    void setTempoUltimoArtefatoAtaque(float tempo);
};
