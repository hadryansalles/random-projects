#include "Animador.hpp"
#include "Fase.hpp"

Animador::Animador()
:   colunas(0)
,   coluna_atual(0)
,	tempo_contabilizado(0.0f)
,	tempo_por_quadro(0.0f)
,	ja_terminou(false)
,	ativa (false) {
}

Animador::Animador(const Animador& copia) 
:   coluna_atual(copia.coluna_atual)
,	tempo_contabilizado(copia.tempo_contabilizado)
,	ja_terminou(copia.ja_terminou)
,	ativa (copia.ativa) {
	textura = copia.textura;
	sprite = copia.sprite;
	colunas = copia.colunas;
	tempo_por_quadro = copia.tempo_por_quadro;
}

Animador::~Animador() {
	colunas = 0;
	coluna_atual = 0;
	tempo_por_quadro = 0.0f;
	tempo_contabilizado = 0.0f;
}

bool Animador::setConfig(std::string caminho, int colunas, float tempo_por_quadro) {
	this->colunas = colunas;
	this->tempo_por_quadro = tempo_por_quadro;

    carregarTextura(caminho);
	tempo_contabilizado = 0.0f;
	coluna_atual = 0.0f;

	sprite.width = textura.getSize().x / colunas;
	sprite.height = textura.getSize().y;

	return true;
}

void Animador::reiniciar() {
	ja_terminou = false;
	ativa = true;
    zerar();
}

void Animador::atualiza(bool para_direita, sf::RectangleShape& corpo, float tempo) {
    // se nao for o valor default utiliza o cronometro da fase
    tempo_contabilizado += (tempo == -1 ? Fase::getCronometro() : tempo);
	if (tempo_contabilizado >= tempo_por_quadro) {
		tempo_contabilizado = 0;
		coluna_atual++;
		if (coluna_atual >= colunas) {
			coluna_atual = 0;
			ja_terminou = true;
			ativa = false;
		}
	}
	if (para_direita) {
		sprite.left = coluna_atual * sprite.width;
		sprite.width = std::abs(sprite.width);
	}
	else {
		sprite.left = (coluna_atual+1) * std::abs(sprite.width);
		sprite.width = - std::abs(sprite.width);
	}
	corpo.setSize(sf::Vector2f((float)sprite.width, (float)sprite.height));
	corpo.setOrigin(sf::Vector2f(sprite.width / 2.0f, sprite.height / 2.0f));
	corpo.setTexture(&textura);
	corpo.setTextureRect(sprite);
}

bool Animador::terminou() const {
	return ja_terminou;
}

bool Animador::executou(int coluna_especifica) const {
	if (coluna_atual == coluna_especifica) {
		return true;
	}
	return false;
}

void Animador::zerar() {
	coluna_atual = 0;
}

int Animador::getLargura() const {
	return textura.getSize().x;
}

void Animador::carregarTextura(std::string caminho) {
	if (!textura.loadFromFile(caminho)) {
        std::cout << "Animador::carregarTextura(string): ";
        std::cerr << "erro carregando ";
        std::cerr << caminho << std::endl;
    }
}

bool Animador::estaAtiva() const {
	return ativa;
}
