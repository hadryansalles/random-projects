#pragma once

#include "Std.hpp"
class Jogador;

class Janela {
private:
	sf::Event evento;
    sf::RenderWindow janela;
	sf::View camera;
	Jogador* _jogador_principal; // janela conhece o jogador principal para sepre acompanha-lo
    const unsigned int _altura;
    const unsigned int _largura;
public:
    Janela (int resolucao_x = 1280, int resolucao_y = 720,
        std::string nome = "Janela", int largura = 384, int altura= 288, int fps_maximo = 60);
    ~Janela();

	// verifica se o X da janela foi apertado,
    // se tiver sido ela é fechada e o programa encerrado
    void atualizar ();

    void resetarCamera();
    std::string& getString (std::string& str, bool receber_dados);
    bool estaAberta();
    void close();

	//limpa a janela, deixando-a transparente
	void clear();
	//desenha o RectangleShape na tela
    void desenhar(sf::Drawable& desenho);
	//troca o buffer para mostrar o novo Frame
    void renderiza();

	//define uma entidade principal que sera acompanhada pela "camera"
    void setJogadorPrincipal(Jogador* p_ent);
	//seta como NULL a entidade principal
	void removerJogadorPrincipal();
	//atualiza a camera para acompanhar a posicao da entidade
	void atualizarCamera(sf::Vector2f limites);
    void atualizarCamera(); //sem argumento considera limites
	//retorna a posicao da camera no mapa
	sf::Vector2f getCameraPos();
};
