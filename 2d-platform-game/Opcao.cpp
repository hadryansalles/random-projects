#include "Opcao.hpp"
#include "Fase.hpp"

Opcao::Opcao()
:   _ativo        ( false )
,   _texto        ( )
,   _tamanho      ( 15 )
,   _ativado      (sf::Color::Red)
,   _desativado   (sf::Color::Black)
,   _string       ("")
,   _pre_fixo     ("") {
    _texto.setFillColor(_desativado);
}

Opcao::Opcao(std::string str, std::string pre_fixo, sf::Color ativado, sf::Color desativado)
:   _ativo        ( false )
,   _texto        ( )
,   _tamanho      ( 15 )
,   _ativado      (ativado)
,   _desativado   (desativado)
,   _string       ( str )
,   _pre_fixo     ( pre_fixo ) {
    desativar();
}
Opcao::~Opcao() {
}

void Opcao::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_texto, states);
} 
void Opcao::ativar() {
    _texto.setString(_pre_fixo + _string);
    _texto.setFillColor(_ativado);
    _texto.setCharacterSize((int)(_tamanho * 1.07f));
	_ativo = true;
}
void Opcao::desativar() {
    _texto.setString(std::string(_pre_fixo.size(),' ') + _string);
    _texto.setFillColor(_desativado);
    _texto.setCharacterSize(_tamanho);
	_ativo = false;
}
bool Opcao::isAtivo() const {
	return _ativo;
}

void Opcao::setFont (const sf::Font& fonte) {
    _texto.setFont(fonte);
    atualizaOrigem();
}
void Opcao::setTamanho(unsigned int tamanho) {
    _tamanho = tamanho;
    _texto.setCharacterSize(_tamanho);
    atualizaOrigem();
}

void Opcao::atualizaOrigem() {
    sf::FloatRect rect = _texto.getLocalBounds();
    setOrigin (0.0f, rect.height / 2.0f);
} 

void Opcao::setString(std::string str) {
    _texto.setString(str);
    _string = str;
} 

int Opcao::getTamanho() const {
    return _tamanho;
} 
void Opcao::setOrigemNoCentro() {
    sf::FloatRect rect = _texto.getLocalBounds();
    setOrigin (rect.width / 2.0f, rect.height / 2.0f);
} 

void Opcao::setColorAtivado (sf::Color cor) {
    _ativado = cor;
} 

void Opcao::setColorDesativado (sf::Color cor) {
    _desativado = cor;
} 
