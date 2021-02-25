#include "EsqueletoNu.hpp"
#include "Fase.hpp"

const float EsqueletoNu::vida ( 0.5f );
const float EsqueletoNu::dano ( 7.0f );
const float EsqueletoNu::valor_morte ( 200.0f );
const float EsqueletoNu::tempo_recarga ( 0.1f );
const sf::Vector2f EsqueletoNu::area ( 18, 38);
const sf::Vector2f EsqueletoNu::velocidade ( 0.3f, 0.0f);

EsqueletoNu::EsqueletoNu(sf::Vector2f posicao, bool do_bem, bool para_direita) 
:   Esqueleto(vida, posicao, do_bem, para_direita, valor_morte)
{
    setIdTipoObjeto ( Codigos::ESQUELETO_NU );
	setVelocidade(velocidade);
	setArea(area);
    setCoeficiente ( 0.8f);
	proprio_corpo = new ArtefatoAtaque(dano, tempo_recarga, area, this);
    proprio_corpo->ativar();
	carregarAnimacoes();
}

EsqueletoNu::~EsqueletoNu() {
}

void EsqueletoNu::mover() {
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

void EsqueletoNu::carregarAnimacoes() {
    std::string caminho_nascendo = "sprites/esqueleto-nascer-sprite.png";
    int numero_colunas_nascendo = 6;
	nascendo.setConfig(caminho_nascendo,
		numero_colunas_nascendo, tempo_recarga);

    std::string caminho_andando = "sprites/esqueleto-sprite.png";
    int numero_colunas_andando = 8;
	andando.setConfig(caminho_andando,
		numero_colunas_andando, tempo_recarga);
}
