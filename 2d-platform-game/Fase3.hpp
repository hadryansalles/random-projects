#pragma once

#include "Std.hpp"
#include "Fase.hpp"
#include "Janela.hpp"
#include "Ladrao.hpp"
#include "Demonio.hpp"

class Fase3 : public Fase{
private:
	sf::RectangleShape _cenario_dinamico;
	sf::Texture _t_cenario_dinamico;

    static const int AlturaChao;
    static const sf::Vector2f SpawnJogador1;
    static const sf::Vector2f SpawnJogador2;

    Demonio* _chefao;

    Demonio* criarDemonio(sf::Vector2f posicao);
    EntidadeColidivel* criarBlocoChao(float pos_x, float pos_y, float escala = 1.0f);

    // Carregar as texturas do cenario
	void carregarCenario();

    // Carregar o jogo de um arquivo
    bool carregar();

public:
    Fase3 (std::string jogador1, std::string jogador2, Janela* janela);
    Fase3 (Janela* janela);
	~Fase3();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // chama os metodos expecificos dessa fase
	void atualizar();
    int getEstado();

    // salva a fase em arquivo
    void salvar();
};
