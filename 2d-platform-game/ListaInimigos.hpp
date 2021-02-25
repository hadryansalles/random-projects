#pragma once
#include "Std.hpp"
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "Lista.hpp"
#include "ListaProjeteis.hpp"
#include "ListaArtefatosAtaque.hpp"

class ListaInimigos
:   public Lista<Inimigo*> {
public:
    ListaInimigos();
    ~ListaInimigos();
    void matarSemVida() const;
    void matarSairamDoCenario(int altura_cenario) const;
    void checarColisoes(const ListaArtefatosAtaque& ataques) const;
    void atacar(ListaProjeteis& projeteis, ListaArtefatosAtaque& ataques) const;
    void removerMortos ();
    std::ofstream& operator >> (std::ofstream& out);
};
