#pragma once

#include "Std.hpp"
#include "Fase1.hpp"
#include "Fase2.hpp"
#include "Fase3.hpp"
#include "Menu.hpp"

class Principal {
private:
	Janela _janela;
	Menu _menu;
	Fase* _fase_atual;

	bool _rodando;
	int _id_fase_atual;
    int _menu_atual;
    bool _jogador_1;
    bool _jogador_2;
    std::string _nome_jogador_1;
    std::string _nome_jogador_2;
    
    // Chama o _menu e trata os retornos do mesmo.
    void menu();

    // Retorna o boleano indicando se o jogo esta
    // rodando, ou seja esta com uma fase ativa
    bool estaRodando();

    // Inicia a fase correspondente ao id
    // Se nao passado os jogadores entao carrega um jogo salvo
    void iniciarJogo(int id_fase, bool jogador_1 = false, bool jogador_2 = false);

    // Desaloca a fase atual
    void destruirFaseAtual();

    void salvar();
    void carregar();

    static const std::string NomeJogo;
    static const sf::Vector2u Resolucao;
    static const int FpsMaximo;
    
public:
	Principal(); 
	~Principal();
    
    // Loop principal do jogo
	void executar();

    // Isso serve apenas para manter as proporcoes corretas
    // do jogo, ainda assim a resolucao pode ser alterada
    static const int Altura;
    static const int Largura;
};
