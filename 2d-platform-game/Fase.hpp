#pragma once
#include "Std.hpp"
#include "Janela.hpp"
#include "Personagem.hpp"
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "ListaEntidadesColidiveis.hpp"
#include "ListaArtefatosAtaque.hpp"
#include "ListaProjeteis.hpp"
#include "ListaInimigos.hpp"
#include "Entidade.hpp"
#include "EsqueletoNu.hpp"
#include "EsqueletoRoupa.hpp"
#include "Fantasma.hpp"

class Fase: public Entidade{
protected:
    class PainelPontuacao : public Entidade {
    private:
        sf::Text _nome_jogador;
        sf::Text _pontuacao;
        sf::Font _fonte;
    public:

    PainelPontuacao(std::string nome_jogador = "Jogador" , unsigned int tamanho = 15);
        ~PainelPontuacao();
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        float getLargura() const ;
        void setNomeJogador(std::string nome);
        void atualizaPontuacao(float pontuacao);
        std::ofstream& operator >> (std::ofstream& out);
        std::ifstream& operator << (std::ifstream& in);
    };

    PainelPontuacao _painel_jogador_1;
    PainelPontuacao _painel_jogador_2;

private:
    void descartarProjeteis();
    void checaColisaoArtefatosAtaquePersonagens();
    void checaColisaoEntidades ();
    void matarPersonagensSemVida();
    void matarPersonagensQueSairamDoCenario();
    void moverEntidades() const;
    void personagensAtacar();
    void destruirEntidadesDescartadas();
    void removerPersonagensMortos();
    void atualizaPainelPontuacao();
    
    void reiniciarCronometro();
    // O jogador que esta na frente sera seguido pela camera
    void definirJogadorPrincipal();

protected:
    // inicio e termino da textura
	sf::Vector2f _area_da_fase;

	ListaEntidadesColidiveis _entidades;
	ListaInimigos _inimigos;
	ListaArtefatosAtaque _artefatos_ataque;
	ListaProjeteis _projeteis;

	sf::Clock _clock;

    int _numero_jogadores;
    Jogador* _jogador_1;
    Jogador* _jogador_2;

    Janela* _janela;
	static float _cronometro;

    sf::Music _musica_de_fundo;
    
    // textuta para as entidades colidiveis
	sf::Texture* _caixa_madeira;
	sf::Texture* _bloco_chao;
    
    sf::IntRect _fim_fase;

    void criarParedes ();
    virtual void criarChao(std::string caminho);
    virtual EntidadeColidivel* criarBlocoChao(float pos_x, float pos_y, float escala, std::string caminho);
    EntidadeColidivel* criarCaixaMadeira(float pos_x = 0.0f, float pos_y = 0.0f);
    EsqueletoNu* criarEsqueletoNu(sf::Vector2f posicao = sf::Vector2f(0.0f, 0.0f));
    EsqueletoRoupa* criarEsqueletoRoupa(sf::Vector2f posicao = sf::Vector2f(0.0f, 0.0f));

    void adicionar(Projetil* projetil);
    void remover(Projetil* projetil);
    void adicionar(ArtefatoAtaque* ataque);
    void remover(ArtefatoAtaque* ataque);
    void adicionar(EntidadeColidivel* entidade);
    void remover(EntidadeColidivel* entidade);
    void adicionar(Inimigo* inimigo);
    void remover(Inimigo* inimigo);

	virtual void carregarCenario() = 0;
    void setMusica(std::string caminho, float volume = 100.f);
    void limparListas();

    void iniciarJogadores(bool jogador1, bool jogador2);
    virtual bool carregar() = 0;
    sf::Texture carregarTextura(std::string caminho);

    bool temJogadorProximo(sf::Vector2f posicao) const;
	void salvarPontuacao() const;
public:
    static const float gravidade;
    static float getCronometro();

public:
    Fase (Janela* janela, sf::IntRect fim_fase = sf::IntRect(0,0,0,0)); 
	virtual ~Fase();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2f getArea() const;

    // chama as funcoes especificas de cada fase
    virtual void atualizar();
   
    // cada fase tem atributos diferentes para salvar
    virtual void salvar() = 0;

    // retorna: PAUSE ou FIM_FASE
    virtual int getEstado();
    std::pair<bool,bool> getExistenciaJogadores() const;

    // voltar o jogo depois do pause
    void continuarJogo();
};
