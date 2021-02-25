#include "EsqueletoRoupa.hpp"
#include "Fase.hpp"

const float EsqueletoRoupa::vida ( 0.5f );
const float EsqueletoRoupa::dano ( 2.0f );
const float EsqueletoRoupa::valor_morte ( 50.0f );
const float EsqueletoRoupa::tempo_recarga ( 0.15f );
const sf::Vector2f EsqueletoRoupa::area ( 18, 38);
const sf::Vector2f EsqueletoRoupa::velocidade (0.6f, 0.0f);

EsqueletoRoupa::EsqueletoRoupa(sf::Vector2f posicao, bool do_bem, bool para_direita) 
:   Esqueleto(vida, posicao, do_bem, para_direita, valor_morte)
{
    setIdTipoObjeto ( Codigos::ESQUELETO_ROUPA );
	setVelocidade(velocidade);
	setArea(sf::Vector2f(area));
    setCoeficiente ( 0.2f);
	proprio_corpo = new ArtefatoAtaque(dano, tempo_recarga, area, this);
    proprio_corpo->ativar();
	carregarAnimacoes();
	nascendo.atualiza(true, _corpo, tempo_recarga);
}
EsqueletoRoupa::~EsqueletoRoupa() {
}
void EsqueletoRoupa::mover() {
	if (!estaVivo() || (getJogador1() == NULL && getJogador2() == NULL)) {
        return;
    }
    if (_esta_nascendo) {
        nascendo.atualiza(estaParaDireita(), _corpo);
        if (nascendo.terminou())
            _esta_nascendo = false;
    }
    else {
        if (getPosicao().x < getJogadorMaisProximo()->getPosicao().x) {
            setEstaParaDireita(false);
            andando.atualiza(estaParaDireita(), _corpo);
            setVelocidade(velocidade.x, getVelocidade().y);
        }
        else {
            setEstaParaDireita(true);
            andando.atualiza(estaParaDireita(), _corpo);
            setVelocidade(-velocidade.x, getVelocidade().y);
        }
    }
    EntidadeColidivel::mover();
}

void EsqueletoRoupa::carregarAnimacoes() {
    std::string caminho_nascendo = "sprites/esqueleto-roupa-nascer-sprite.png";
    int numero_colunas_nascendo = 6;
	nascendo.setConfig(caminho_nascendo,
		numero_colunas_nascendo, tempo_recarga);

    std::string caminho_andando = "sprites/esqueleto-roupa-sprite.png";
    int numero_colunas_andando = 8;
	andando.setConfig(caminho_andando,
		numero_colunas_andando, tempo_recarga);
}
