#pragma once
#include "ArtefatoAtaque.hpp"
#include "Animador.hpp"

class Projetil : public ArtefatoAtaque {
private:
	Animador _movendo;
	bool para_direita;
    bool _descartavel;

public:
	Projetil(float dano, float tempo_de_recarga, sf::Vector2f alcance,
		Personagem* dono);
    Projetil(const Projetil& copia);
	virtual ~Projetil();
	void setAnimador(std::string caminho, int colunas, float tempo_por_quadro);
	virtual void mover();
    bool isDescartavel() const;
    void setDescartavel(bool desativavel);
    std::ofstream& operator >> (std::ofstream& out);
    std::ifstream& operator << (std::ifstream& in);
};
