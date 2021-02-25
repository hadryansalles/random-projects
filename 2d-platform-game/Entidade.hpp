#pragma once
#include "Std.hpp"

// Esta classe abstrata tem por finalidade servir de base para
// as demais classes do jogo que sao passiveis de serem desenhadas
// na tela e/ou sofrerem transformacoes (mudanca de posicao, origem etc)

class Entidade: public sf::Drawable, public sf::Transformable {
public:
    Entidade();
    virtual ~Entidade();

    // Funcao a ser redefinida em cada classe "desenhavel"
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const = 0;
};
