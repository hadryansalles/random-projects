#include "Janela.hpp"
#include "Jogador.hpp"

Janela::Janela (int resolucao_x, int resolucao_y, std::string nome, int largura, int altura, int fps_maximo)
:	janela(sf::VideoMode(largura, altura), nome, sf::Style::Titlebar | sf::Style::Resize)
,	camera(sf::Vector2f(0.0f,0.0f), sf::Vector2f(largura, altura)) 
,   _jogador_principal(NULL)
,   _altura (altura)
,   _largura (largura)
{ 
	janela.setPosition(sf::Vector2i(0,0));
	janela.setSize(sf::Vector2u(resolucao_x, resolucao_y));
    janela.setFramerateLimit(fps_maximo);
}

Janela::~Janela() {
	if (janela.isOpen()) {
		janela.close();
    }
}

bool Janela::estaAberta() {
    return janela.isOpen();
}
void Janela::atualizar () {
    while (janela.pollEvent(evento)) {
//         Caso foi clicado no 'X' da janela
        if (evento.type == sf::Event::Closed) {
            janela.close();
        }
    }
}

void Janela::close() {
    janela.close();
}
void Janela::clear() {
    // "Limpa a tela" de branco
    janela.clear(sf::Color::Transparent);
}

void Janela::setJogadorPrincipal(Jogador* p_ent) {
	
	if(p_ent != NULL) {
		_jogador_principal = p_ent;
    }
	else {
		std::cerr << "Janela::setJogadorPrincipal: ponteiro para jogador invalido" << std::endl;
	}
}

void Janela::removerJogadorPrincipal() {
    resetarCamera();
	_jogador_principal = NULL;
}
void Janela::resetarCamera() {
	camera.setCenter(sf::Vector2f(_largura / 2, _altura/2));
	janela.setView(camera);
}
void Janela::atualizarCamera(sf::Vector2f limites) {
	if (_jogador_principal != NULL) {
        int delta_direita = _largura / 10;
        int delta_esquerda = _largura / 3;
        if (_jogador_principal->getPosicao().x > limites.x + _largura * 0.26 - delta_direita && 
			_jogador_principal->getPosicao().x < limites.y - _largura * 0.735 + delta_esquerda) {
                if (_jogador_principal->getPosicao().x - delta_direita > camera.getCenter().x ) {
                        camera.setCenter(_jogador_principal->getPosicao().x - delta_direita, _altura/2);
                }
                else if (_jogador_principal->getPosicao().x + delta_esquerda < camera.getCenter().x ) {
                    camera.setCenter(_jogador_principal->getPosicao().x + delta_esquerda , _altura/2);
                }
                janela.setView(camera);
        }
	}
}
void Janela::atualizarCamera() {
	if (_jogador_principal != NULL) {
        if (_jogador_principal->getPosicao().x - 40 > camera.getCenter().x ) {
            camera.setCenter(_jogador_principal->getPosicao().x - 40, _altura/2);
        }
        else if (_jogador_principal->getPosicao().x + 100 < camera.getCenter().x ) {
            camera.setCenter(_jogador_principal->getPosicao().x + 100, _altura/2);
        }
        janela.setView(camera);
    }
}
void Janela::desenhar(sf::Drawable& desenho) {
	janela.draw(desenho);
}

void Janela::renderiza() {
    janela.display();
}

sf::Vector2f Janela::getCameraPos() {
	return camera.getCenter();
}

std::string& Janela::getString (std::string& str, bool receber_dados) {
    while (janela.pollEvent(evento)) {
        // Permite fechar a janela enquanto recebe a string
        if (evento.type == sf::Event::Closed) {
            janela.close();
        }
        if (evento.type == sf::Event::TextEntered) {
            if (evento.text.unicode < 128 && receber_dados) {
                char c = static_cast<char>(evento.text.unicode);
                if (c == '\b' && str.size() > 0) {
                    str.pop_back();
                }
                else if (c != 13 && c != '\b') {
                    str += c;
                }
            }
        }
    }
    return str;
}
