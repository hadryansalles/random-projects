#pragma once
#include "Std.hpp"
#include "Projetil.hpp"
#include "Lista.hpp"

class Inimigo;

class ListaProjeteis 
:   public Lista<Projetil*>
,   public Entidade {
public:
    ListaProjeteis();
    ~ListaProjeteis();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void mover() const;
    void descartarDesativados() const ;
    void descartarSairamDoCenario(int altura_cenario) const;
    void removerDescartados();
    std::ofstream& operator >> (std::ofstream& out);
};
