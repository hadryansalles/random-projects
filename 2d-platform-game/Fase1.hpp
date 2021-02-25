#pragma once

#include "Std.hpp"
#include "Fase.hpp"
#include "Janela.hpp"

class Fase1 : public Fase{

private:
	// cenario em primeiro plano
	sf::RectangleShape _cenario_estatico;
	// cenario em segundo plano
	sf::RectangleShape _cenario_dinamico;
	
	sf::Texture _t_cenario_estatico;
	sf::Texture _t_cenario_dinamico;

    std::vector<sf::Vector2f> _pos_esqueletos_roupa;
    std::vector<sf::Vector2f> _pos_esqueletos_nu;
    std::vector<sf::Vector2f> _pos_fantasmas;

	sf::Texture* _chao;

    static const int AlturaChao;
    static const sf::Vector2f SpawnJogador1;
    static const sf::Vector2f SpawnJogador2;

    // Spawnar as entidades pelo cenario
    void spawnarInimigos();
    void spawnarFantasmas();
    void spawnarEsqueletosRoupa();
    void spawnarEsqueletosNu();
    void spawnarCaixasMadeira();

    Fantasma* criarFantasma(sf::Vector2f posicao = sf::Vector2f(0.0f, 0.0f));
    EntidadeColidivel* criarBlocoChao(float pos_x, float pos_y, float escala = 1.0f);

    // Sorteia onde os esqueletos deveram spawnar
    void sorteiaPosicaoEntidades();

    // Criar Objetos
    void criarChao();
    void criarMarquises();

    // Carregar as texturas do cenario
	void carregarCenario();

    // Carregar o jogo de um arquivo
    bool carregar();

public:
    Fase1 (std::string jogador1, std::string jogador2, Janela* janela);
    Fase1 (Janela* janela);
	~Fase1();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // chama os metodos expecificos dessa fase
	void atualizar();

    // salva a fase em arquivo
    void salvar();
};
