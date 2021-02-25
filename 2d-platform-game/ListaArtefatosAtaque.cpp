#include "ListaArtefatosAtaque.hpp"
#include "Inimigo.hpp"

ListaArtefatosAtaque::ListaArtefatosAtaque() {

}

ListaArtefatosAtaque::~ListaArtefatosAtaque() {
    limparLista();
}

void ListaArtefatosAtaque::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const_iterator it = cbegin(); it != cend(); it++) {
        if ((*it)->ehProjetil()) {
            target.draw(*(*it), states);
        }
    }
}

void ListaArtefatosAtaque::limparLista() {
    iterator it = begin();
    while (it != end()) {
        ArtefatoAtaque*& ent = *it;
        if (ent != NULL) {
            delete ent;
            ent = NULL;
        } 
        it++;
    }
}

void ListaArtefatosAtaque::checarColisao(Inimigo* inimigo) const {
    const_iterator ataque = cbegin();
    while (ataque != cend()) {
        if ( (*ataque)->getAtivado()
          && (*ataque)->getDono() != NULL
          && inimigo->getDoBem() != (*ataque)->getDono()->getDoBem()
          && colidiu(*ataque, inimigo)) {
            (*ataque)->atacar ( inimigo );
        }
        ataque++;
    }
}

void ListaArtefatosAtaque::checarColisao(Jogador* jogador) const {
    const_iterator ataque = cbegin();
    while (ataque != cend()) {
        if ( (*ataque)->getAtivado()
            && jogador != (*ataque)->getDono()
            && colidiu(*ataque, jogador)) {
            (*ataque)->atacar( jogador );
        }
        ataque++;
    }
}

bool ListaArtefatosAtaque::colidiu (ArtefatoAtaque* ataque, Personagem* personagem) const {
    float _distancia_x = ataque->getPosicao().x - personagem->getPosicao().x;
    float _distancia_y = ataque->getPosicao().y - personagem->getPosicao().y;
    float _interseccao_x = std::abs(_distancia_x) - (ataque->getArea().x/2 + personagem->getArea().x/2);
    float _interseccao_y = std::abs(_distancia_y) - (ataque->getArea().y/2 + personagem->getArea().y/2);
    if (_interseccao_x <= 0.0f && _interseccao_y <= 0.0f) {
        return true;
    }
    return false;
}

void ListaArtefatosAtaque::destruirDescartados() {
    iterator ataque = begin();
    while (ataque != end()) {
        if ((*ataque)->isDescartado()) {
            ataque = destruir ( ataque );
        }
        ataque++;
    }
}

ListaArtefatosAtaque::iterator ListaArtefatosAtaque::destruir(ListaArtefatosAtaque::iterator& artefato) {
    if (*artefato != NULL) {
        delete (*artefato);
    }
    return ( erase (artefato));
}
