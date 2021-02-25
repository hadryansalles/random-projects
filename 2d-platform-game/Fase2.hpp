#pragma once

#include "Std.hpp"
#include "Fase.hpp"
#include "Janela.hpp"
#include "Ladrao.hpp"
#include "Demonio.hpp"

class Fase2 : public Fase{
private:
    // local onde o jogador encerra a fase
	sf::RectangleShape _portal_fim_fase;
	// cenario em segundo plano
	sf::RectangleShape _cenario_dinamico;
	
	sf::Texture _t_portal_fim_fase;
	sf::Texture _t_cenario_dinamico;

    std::vector<sf::Vector2f> _pos_esqueletos_roupa;
    std::vector<sf::Vector2f> _pos_esqueletos_nu;
    std::vector<sf::Vector2f> _pos_fantasmas;
    std::vector<sf::Vector2f> _pos_ladroes;
    std::vector<sf::Vector2f> _pos_espinhos;
    std::vector<sf::Vector2f> _pos_caixas;
	sf::Texture* _espinho;

    static const int AlturaChao;
    static const sf::Vector2f SpawnJogador1;
    static const sf::Vector2f SpawnJogador2;

    // Spawnar as entidades pelo cenario
    void spawnarInimigos();
    void spawnarFantasmas();
    void spawnarLadroes();
    void spawnarEsqueletosRoupa();
    void spawnarEsqueletosNu();
    void spawnarCaixasMadeira();
    void spawnarEspinhos();

    Fantasma* criarFantasma(sf::Vector2f posicao = sf::Vector2f(0,0));
    Ladrao* criarLadrao(sf::Vector2f posicao = sf::Vector2f(0,0));
    Projetil* criarEspinho(sf::Vector2f posicao = sf::Vector2f(0,0));

    // Sorteia onde os esqueletos deveram spawnar
    void sorteiaPosicaoEntidades();

    // Carregar as texturas do cenario
	void carregarCenario();

    // Carregar o jogo de um arquivo
    bool carregar();

public:
    Fase2 (std::string jogador1, std::string jogador2, Janela* janela);
    Fase2 (Janela* janela);
	~Fase2();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // chama os metodos expecificos dessa fase
	void atualizar();

    // salva a fase em arquivo
    void salvar();
};
