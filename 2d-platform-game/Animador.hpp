#pragma once

#include "Std.hpp"
class Fase;
class Animador {

private:
	sf::Texture textura;
	sf::IntRect sprite;
	int colunas;
	int coluna_atual;
	float tempo_contabilizado;
	float tempo_por_quadro;
	bool ja_terminou;
	bool ativa;
    void carregarTextura(std::string caminho);

public:
	Animador();
	Animador(const Animador& copia);
	~Animador();

	//configura valores iniciais para a anima�ao
	bool setConfig(std::string caminho, int colunas, float tempo_por_quadro);	

	void atualiza(bool para_direita, sf::RectangleShape& corpo, float tempo = -1);

	//retorna booleano caso a anima�ao tenha atinjido o quadro de execu�ao
	bool executou(int coluna_especifica) const;

	//retorna booleano case a anima�ao tenha terminado (chegou ao ultimo sprite)
	bool terminou() const;

    //volta a anima�ao para o primeiro quadro
	void zerar();

	int getLargura() const;
	void reiniciar();
	bool estaAtiva() const;
};
