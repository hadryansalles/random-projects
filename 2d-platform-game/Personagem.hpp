#pragma once

#include "Std.hpp"
#include "EntidadeColidivel.hpp"
#include "Animador.hpp"
#include "ArtefatoAtaque.hpp"
#include "Projetil.hpp"

class Personagem : public EntidadeColidivel {
private:
    // Distingue os jogadores dos inimigos
	bool _do_bem;
	bool _vivo;

    // Som quando o personagem recebe um dano
    sf::SoundBuffer _b_receber_dano;
    sf::Sound _receber_dano;

protected:
	Animador _morte;
	float _vida;

    // Indica para que lado o personagem esta virado
	bool _para_direita;

    void setEstaParaDireita(bool para_direita);
    bool estaParaDireita();
	void setDoBem(bool do_bem);

public:
    Personagem(float vida = 1.0f, bool do_bem = true,
        bool para_direita = true, bool vivo = true);

    virtual ~Personagem();

	virtual Projetil* atacar() = 0;

	virtual ArtefatoAtaque* getArtefatoAtaque() const;
	
	virtual void diminuirVidaEm(float dano);

	float getVida() const ;
    void setVida( float vida);
	bool getDoBem();
	void matar();
	bool morreu();
	bool estaVivo();

    float getPontuacao() const;
    float getValorMorte() const;
    void alterarPontuacao(const float delta);

    void setSomDano(std::string caminho);
};
