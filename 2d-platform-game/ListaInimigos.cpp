#include "ListaInimigos.hpp"

ListaInimigos::ListaInimigos() {
}

ListaInimigos::~ListaInimigos() {
}

void ListaInimigos::matarSemVida() const {
    const_iterator inimigo = cbegin();
    while (inimigo != cend()) {
		if ((*inimigo) != NULL && (*inimigo)->getVida() <= 0 && (*inimigo)->estaVivo()) {
			(*inimigo)->matar();
		}
        ++inimigo;
    }
}

void ListaInimigos::matarSairamDoCenario(int altura_cenario) const {
    const_iterator inimigo = cbegin();
    while (inimigo != cend()) {
		if ((*inimigo)->getPosicao().y > altura_cenario - (*inimigo)->getArea().y/2.0f && (*inimigo)->estaVivo()) {
            (*inimigo)->setVelocidade (0.0f, 0.0f);
			(*inimigo)->matar();
		}
        ++inimigo;
    }
}

void ListaInimigos::checarColisoes(const ListaArtefatosAtaque& ataques) const {
    const_iterator inimigo = cbegin();
    while (inimigo != cend()) {
        ataques.checarColisao (*inimigo);
        inimigo++;
    }
}

void ListaInimigos::atacar(ListaProjeteis& projeteis, ListaArtefatosAtaque& ataques) const {
	Projetil* aux;
    const_iterator inimigo = cbegin();
	while (inimigo != cend()) {
		 // atacar retorna NULL caso o personagem
         // nao possua para incluir
         aux = (*inimigo)->atacar();
		 if (aux != NULL) {
			 projeteis.push_back(aux);
			 ataques.push_back(static_cast<ArtefatoAtaque*> (aux));
		 }
         ++inimigo;
    }
}

void ListaInimigos::removerMortos () {
    iterator inimigo = begin();
    while (inimigo != end()) {
		if ((*inimigo)->morreu()) {
            (*inimigo)->descartar();
            inimigo = erase (inimigo);
        }
        else inimigo++;
    }
}

std::ofstream& ListaInimigos::operator >> (std::ofstream& out) {
    iterator inimigo = begin();
    while (inimigo != end()) {
        *(*inimigo) >> out;
        out << '\n';
        ++inimigo;
    }
    return out;    
}
