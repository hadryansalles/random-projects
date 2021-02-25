#pragma once
#include "Std.hpp"
#include "ArtefatoAtaque.hpp"
#include "Entidade.hpp"
#include "Lista.hpp"

class Inimigo;
class ListaArtefatosAtaque 
:   public Lista<ArtefatoAtaque*> {
private:
    bool colidiu (ArtefatoAtaque* ataque, Personagem* personagem) const;

public:
    ListaArtefatosAtaque();
    ~ListaArtefatosAtaque();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void checarColisao(Jogador* jogador) const;
    void checarColisao(Inimigo* inimigo) const;
    void destruirDescartados();

    void limparLista();
    ListaArtefatosAtaque::iterator destruir(ListaArtefatosAtaque::iterator& artefato);
};
