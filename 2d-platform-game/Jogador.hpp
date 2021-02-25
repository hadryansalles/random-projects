#pragma once
#include "Std.hpp"
#include "Personagem.hpp"
#include "Projetil.hpp"

class Fase;

class Jogador : public Personagem {
private:
    class BarraDeVida: Entidade {
    private:
        sf::VertexArray _barra_de_vida;
        int _n_divisoes;
    public:
        void setConfiguracoes(sf::Vector2f pos, float largura, int n_divisoes);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        BarraDeVida();
        ~BarraDeVida();
        void setColor (const sf::Color& cor);
        void atualizar(float vida, sf::Vector2f pos);
    };

    static int quantidade;
	bool _esta_atacando;
	bool _esta_tomando_dano;
	bool _esta_imune;
	bool _esta_pulando;

	float _tempo_imune;

	sf::Keyboard::Key _esquerda;
	sf::Keyboard::Key _direita;
	sf::Keyboard::Key _cima;
	sf::Keyboard::Key _baixo;
	sf::Keyboard::Key _t_ataque1;

	Animador _parado;
	Animador _correndo;
	Animador _atacando;
	Animador _tomando_dano;
	Animador _pulando_parado;
	Animador _pulando_correndo;

	Animador* p_animacao_atual;

	ArtefatoAtaque* _espada;
    Jogador::BarraDeVida _barra_de_vida;

    float _pontuacao;
    std::string _nome;

    sf::Sound _som_espada;
    sf::SoundBuffer _som_espada_buffer;
    sf::Sound _som_pulo;
    sf::SoundBuffer _som_pulo_buffer;

    // teclas usadas para movimentar o personagem
	virtual void carregarComandos();

	//carrega texturas do personagem
	void carregarTexturas();
    
    static const float VidaInicial;
    static const float VelocidadeMaxima;
public:
	//inicializa jogador de acordo com player 1 ou 2 
	Jogador(int tipo = -1);
	~Jogador();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void mover();

	virtual Projetil* atacar();
	virtual ArtefatoAtaque* getArtefatoAtaque() const;

	void diminuirVidaEm(float dano);
	bool estaEmAnimacaoDePulo();

    float getPontuacao() const;
    void alterarPontuacao(float delta);
    void setNome(std::string nome);
    std::string getNome() const;

    // salvar e restaurar dados de arquivos
    std::ofstream& operator >> (std::ofstream& out);
    std::ifstream& operator << (std::ifstream& in);
};
