#pragma once

#include "Std.hpp"
#include "EntidadeColidivel.hpp"

class Fase;

class Opcao : public Entidade {
private:
	bool _ativo;
	sf::Text _texto;
    int _tamanho;
    sf::Color _ativado;
    sf::Color _desativado;
    std::string _string;
    std::string _pre_fixo;

    void atualizaOrigem();
public:
    Opcao();
    Opcao(std::string str, std::string _pre_fixo = "", sf::Color ativado = sf::Color::Red, sf::Color desativado = sf::Color::Black);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	~Opcao();
	void ativar();
	void desativar();
	bool isAtivo() const;
    void setFont (const sf::Font& fonte);
    void setTamanho(unsigned int tamanho);
    int getTamanho() const;
    void setString(std::string str);
    void setOrigemNoCentro();
    void setColorAtivado (sf::Color cor);
    void setColorDesativado (sf::Color cor);
};
