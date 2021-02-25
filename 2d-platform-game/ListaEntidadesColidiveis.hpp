#pragma once
#include "Std.hpp"
#include "EntidadeColidivel.hpp"
#include "Lista.hpp"

class ListaEntidadesColidiveis
:   public Lista<EntidadeColidivel*>
,   public Entidade {

private:
    iterator destruir(iterator& entidade);
    void destruir(EntidadeColidivel*& entidade);

public:
    ListaEntidadesColidiveis();
    ~ListaEntidadesColidiveis();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void mover() const;
    void moverPorColisoes() const;
    void destruirDescartados ();
    void limparLista();
    std::ofstream& operator >> (std::ofstream& out) const;
};
