#include "ListaEntidadesColidiveis.hpp"

ListaEntidadesColidiveis::ListaEntidadesColidiveis() {
}

ListaEntidadesColidiveis::~ListaEntidadesColidiveis() {
    limparLista();
    clear();
}

void ListaEntidadesColidiveis::limparLista() {
    iterator it = begin();
    while (it != end()) {
        EntidadeColidivel*& ent = *it;
        if (ent != NULL) {
            delete ent;
            ent = NULL;
        } 
        it++;
    }
}

void ListaEntidadesColidiveis::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const_iterator it = cbegin(); it != cend(); it++) {
        target.draw(*(*it), states);
    }
}

void ListaEntidadesColidiveis::mover() const {
    for (const_iterator ent = cbegin(); ent != cend(); ent++) {
        ent->mover();
    }
}

void ListaEntidadesColidiveis::moverPorColisoes() const {
    sf::Vector2f distancia, interseccao, move_1, move_2, vel_1, vel_2;
    float coeficiente;
    for (const_iterator ent1 = cbegin(); ent1 != cend(); ent1++) {
        for (const_iterator ent2 = ent1; ent2 != cend(); ent2++) {
            if (ent1 != ent2) {
                distancia.x = (*ent1)->getPosicao().x - (*ent2)->getPosicao().x;
                distancia.y = (*ent1)->getPosicao().y - (*ent2)->getPosicao().y;
                interseccao.x = std::abs(distancia.x) - ((*ent1)->getArea().x/2 + (*ent2)->getArea().x/2);
                interseccao.y = std::abs(distancia.y) - ((*ent1)->getArea().y/2 + (*ent2)->getArea().y/2);
                coeficiente = (*ent1)->getCoeficiente() / ((*ent1)->getCoeficiente() + (*ent2)->getCoeficiente());
                if (interseccao.x <= 0.0f && interseccao.y <= 0.0f) { // colidiu
                    if ((*ent1)->getEstatico()) {
                        coeficiente = 1.0f;
                    }
                    else if ((*ent2)->getEstatico()) {
                        coeficiente = 0.0f;
                    }
                    if (interseccao.x > interseccao.y) { // colisao ocorreu no eixo x
                        if (distancia.x < 0.0f) { // (*ent2) esta na direita
                            move_1 = sf::Vector2f (interseccao.x*(1.0f - coeficiente), 0.0f);
                            move_2 = sf::Vector2f (-interseccao.x * coeficiente, 0.0f);
                        }
                        else { // (*ent1) esta na direita
                            move_1 = sf::Vector2f (-interseccao.x * (1.0f - coeficiente), 0.0f);
                            move_2 = sf::Vector2f (interseccao.x * coeficiente,  0.0f);
                        }
                        vel_1 = sf::Vector2f (0.0f, (*ent1)->getVelocidade().y);
                        vel_2 = sf::Vector2f (0.0f, (*ent2)->getVelocidade().y);
                    }
                    else { // colisao ocorreu no eixo y
                        if (distancia.y < 0.0f) { // (*ent1) esta em cima
                            move_1 = sf::Vector2f (0.0f, interseccao.y*(1.0f - coeficiente));
                            move_2 = sf::Vector2f (0.0f, -interseccao.y * coeficiente);
                            vel_2 = sf::Vector2f ((*ent2)->getVelocidade() / 3.0f);
                            vel_1 = sf::Vector2f (0.0f, 0.0f);
                        }
                        else { // (*ent2) esta em cima
                            move_1 = sf::Vector2f (0.0f, -interseccao.y * (1.0f - coeficiente));
                            move_2 = sf::Vector2f (0.0f, interseccao.y * coeficiente);
                            vel_1 = sf::Vector2f ((*ent1)->getVelocidade() / 3.0f);
                            vel_2 = sf::Vector2f (0.0f, 0.0f);
                        }
                    }
                    (*ent1)->moverPorColisao (move_1, vel_1);
                    (*ent2)->moverPorColisao (move_2, vel_2);
                }
            }
        }
    }
}

void ListaEntidadesColidiveis::destruirDescartados () {
    iterator entidade = begin();
    while (entidade != end()) {
		if ((*entidade)->isDescartado()) {
            entidade = destruir (entidade);
        }
        else entidade++;
    }
}

ListaEntidadesColidiveis::iterator ListaEntidadesColidiveis::destruir(iterator& entidade) {
    if (*entidade != NULL) {
        delete (*entidade);
        *entidade = NULL;
    }
    return ( erase (entidade));
}

std::ofstream& ListaEntidadesColidiveis::operator >> (std::ofstream& out) const {
    int tipo ( 0 );
    const_iterator entidade = cbegin();
    while (entidade != cend()) {
        tipo = (*entidade)->getIdTipoObjeto();    
        switch (tipo) {
        case Codigos::CAIXA_MADEIRA:
            *(*entidade) >> out;
            out << '\n';
            break;
        }
        ++entidade;
    }
    return out;    
} 
