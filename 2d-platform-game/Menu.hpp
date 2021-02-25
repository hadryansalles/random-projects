#pragma once

#include "Std.hpp"
#include "Opcao.hpp"
#include "Entidade.hpp"
#include "Ranking.hpp"

class Menu: public Entidade{
private:
    // delay minimo entre um pressionar de tecla e outro
    float _delay_para_escolha;

    // Clock para gerenciar o delay
    sf::Clock _tempo_passado;

    // cores para as opcoes
    sf::Color _ativado;
    sf::Color _desativado;
    
    // Todas as opcoes suportados pelo menu
    Opcao _novo_jogo;
    Opcao _carregar_anterior;
    Opcao _mostrar_ranking;
    Opcao _um_jogador;
    Opcao _dois_jogadores;
    Opcao _voltar_jogo;
    Opcao _salvar;
    Opcao _menu_inicial;
    Opcao _reiniciar_fase;
    Opcao _proxima_fase;
    Opcao _sair_jogo;
    Opcao _fase_1;
    Opcao _fase_2;
    Opcao _fase_3;
    Opcao _digite_nome_1;
    Opcao _digite_nome_2;
    Opcao _nome_jogador;
    Opcao _confirmar;
    Opcao _msg_fim_jogo;

    // Vetor com as opcoes do menu atual
    // cada menu quando eh chamado limpa
    // o vetor e coloca as suas opcoes
    std::vector<Opcao*> _opcoes;

    // fonte padrao
    sf::Font _fonte_opcoes;

    // teclas 
    sf::Keyboard::Key _cima;
    sf::Keyboard::Key _baixo;
    sf::Keyboard::Key _enter;
    sf::Keyboard::Key _escape;
    
    // retangulo para cobrir todo o fundo e
    // imagem que de fundo
	sf::RectangleShape _fundo_cor;
    sf::Texture* _t_imagem_fundo;
    sf::Texture* _t_imagem_game_over;
	sf::RectangleShape _imagem_fundo;
	sf::RectangleShape _imagem_game_over;

    // bip ao mudar de opcao
    sf::SoundBuffer _bip_buffer;
    sf::Sound _bip;
    
    // som ao selecionar uma opcao
    sf::SoundBuffer _enter_sound_buffer;
    sf::Sound _enter_sound;
    
    int _opcao_atual;
    int _menu_atual;
    // Atualiza a posicao das opcoes do menu atual
    // conforme a quantidade de opcoes
    void atualizaPosicaoOpcoes(int escala = 3);
    
    // desativa todas as opcoes
    void desativarOpcoes();

    // algoritimo de escolha, permite alterar entre
    // as opcoes e retorna qual das opcoes do vetor
    // foi selecionada pelo usuario
	int escolha();

    Ranking pontuacoes;
//  Opcoes:
//      Novo Jogo
//      Carregar Jogo
//      Mostrar Ranking
    int inicial();

//  Opcoes:
//      Um Jogador
//      Dois Jogadores
	int jogadores();

//  Opcoes:
//      Voltar Jogo
//      Salvar Jogo
//      Menu Inicial
    int pause();

//  Opcoes:
//      Fase 1
//      Fase 2
//      Fase 3
    int fase();

//  Opcoes:
//      Reiniciar Fase
//      Proxima Fase
//      Menu inicial
//      Sair do Jogo
    int fimFase();

//  Opcoes:
//      Menu inicial
//      Sair do Jogo
    int fimJogo();

//  Opcoes: (imagem de fundo de game over)
//      Menu inicial
//      Sair do Jogo
    int gameOver();

    // sem Opcaoes apenas mostra as pontuacoes
    // obtidas pelo Ranking
    int mostrarRanking();
    
    // menu para receber o nome do jogador,
    // permite confimar e mostra o nome obtido
    // no loop de eventos no centro da tela
    int getNomeJogador1(const std::string& str);
    int getNomeJogador2(const std::string& str);
    
    // Uma vez alocados os objetos Opcao pelo Ranking
    // sera desalocado ao sair do menu mostrar ranking
    void destruirOpcoesRanking();

public:
	Menu();
	~Menu();

	// desenha o menu e suas opcoes
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // recebe o tipo de menu requerido e retorna a opcao selecionada pelo usuario
    int getOpcao(int tipo_menu, std::string& nome_jogador);
    int getOpcao(int tipo_menu);
};
