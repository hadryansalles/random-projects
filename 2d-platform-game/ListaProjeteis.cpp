#include "ListaProjeteis.hpp"
#include "Inimigo.hpp"

ListaProjeteis::ListaProjeteis() {

} 
ListaProjeteis::~ListaProjeteis() {
    clear();
} 
void ListaProjeteis::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const_iterator it = cbegin(); it != cend(); it++) {
        target.draw(*(*it), states);
    }
}

void ListaProjeteis::mover() const {
    for (const_iterator proj = cbegin(); proj != cend(); proj++) {
        proj->mover();
    }
}

void ListaProjeteis::descartarDesativados() const {
	const_iterator it = cbegin();
	while (it != cend()) {
		if (!(*it)->getAtivado()) {
            if ((*it)->isDescartavel()) {
                (*it)->descartar();
            }
            else {
                (*it)->ativar();
            }
        }
		it++;
    }
}

void ListaProjeteis::removerDescartados() {
	iterator it = begin();
	while (it != end()) {
		if ((*it)->isDescartado()) {
            it = erase (it);
		}
		else it++;
	}
}

void ListaProjeteis::descartarSairamDoCenario(int altura_cenario) const {
    const_iterator projetil = cbegin();
    while (projetil != cend()) {
		if ((*projetil)->getPosicao().y > altura_cenario - (*projetil)->getArea().y/2.0f) {
            (*projetil)->setVelocidade (0.0f, 0.0f);
			(*projetil)->descartar();
		}
        ++projetil;
    }
}

std::ofstream& ListaProjeteis::operator >> (std::ofstream& out) {
    iterator projetil = begin();
    while (projetil != end()) {
        *(*projetil) >> out;
        out << '\n';
        ++projetil;
    }
    return out;    
}
