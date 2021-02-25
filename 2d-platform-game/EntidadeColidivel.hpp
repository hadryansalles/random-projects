#pragma once

#include "Std.hpp"
#include "Entidade.hpp"

class EntidadeColidivel : public Entidade{
protected:
    // codigo indicando o tipo do objeto para salvar
    int _id_tipo_objeto;

    // indica a dificuldade para mover a
    // entidade na colisao
	float _coeficiente;

	sf::Vector2f _velocidade;
	sf::Vector2f _aceleracao;

    // usada para realizar a colsisao
	sf::Vector2f _area;
	sf::RectangleShape _corpo;

    // indica se sofre acao da gravidade
    bool _estatico; 

    // indica que nao esta mais em uso e pode ser deletado
    bool _descartado;


public:
	EntidadeColidivel (int tipo = -1, float coeficiente = 0.8f);
    EntidadeColidivel (int tipo, sf::Texture* textura, float pos_x = 0.0f,
        float pos_y = 0.0f, float coeficiente = 0.8f, bool estatico = false);

    virtual ~EntidadeColidivel();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    virtual void mover();
    void moverPorColisao(sf::Vector2f delta, sf::Vector2f velocidade);


    EntidadeColidivel* setPosicao(float x, float y);
    EntidadeColidivel* setPosicao(sf::Vector2f pos);
    sf::Vector2f getPosicao() const ;
    sf::Vector2f getPosicaoDoCanto() const ;

    EntidadeColidivel* setSize(sf::Vector2f tamanho);
    sf::Vector2f getSize() const;

    EntidadeColidivel* setOrigem(float x, float y);
    EntidadeColidivel* setOrigem(sf::Vector2f pos);
    sf::Vector2f getOrigem() const;

    EntidadeColidivel* setTextura (const sf::Texture* textura);
    sf::Texture* carregarTextura(std::string caminho) const;

    EntidadeColidivel* setArea(sf::Vector2f area);
    sf::Vector2f getArea() const ;

    EntidadeColidivel* setVelocidade(sf::Vector2f velocidade);
    EntidadeColidivel* setVelocidade(float x, float y);
    sf::Vector2f getVelocidade() const;

    EntidadeColidivel* setAceleracao(sf::Vector2f aceleracao);
    EntidadeColidivel* setAceleracao(float x, float y);
    sf::Vector2f getAceleracao() const;

    EntidadeColidivel* setCoeficiente(float coeficiente);
    float getCoeficiente() const;

    EntidadeColidivel* setEstatico(bool estatico);
    bool getEstatico() const;
    
    EntidadeColidivel* setIdTipoObjeto(int tipo);

    EntidadeColidivel* setPossuiDono(bool possuiDono);
    bool possuiDono() const;

    void descartar();
    bool isDescartado () const;
    int getIdTipoObjeto() const;
    float distanciaEuclidiana(const EntidadeColidivel* const entidade) const;
    float distanciaEuclidiana(sf::Vector2f posicao) const;

    std::ifstream& operator << (std::ifstream& in);
    std::ofstream& operator >> (std::ofstream& out) const;
};
