#include "EntidadeColidivel.hpp"
#include "Fase.hpp"

EntidadeColidivel::EntidadeColidivel(int tipo, float coeficiente)
:   _id_tipo_objeto ( tipo )
,   _coeficiente ( coeficiente ) 
,   _velocidade (0.0f, 0.0f)
,   _aceleracao (0.0f, 0.0f)
,	_area (0.0f, 0.0f)
,	_corpo()
,   _estatico ( false ) 
,   _descartado ( false ) {
    setPosicao (0.0f , 0.0f);
}
EntidadeColidivel::~EntidadeColidivel() {
}

void EntidadeColidivel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_corpo, states);
}
void EntidadeColidivel::mover() {
    if ( !_estatico ) {
        move(_velocidade);
        _velocidade += sf::Vector2f(0.0f, Fase::gravidade);
    }
} 
void EntidadeColidivel::moverPorColisao(sf::Vector2f delta, sf::Vector2f velocidade) {
    if ( !_estatico ) {
        move (delta);
        setVelocidade (velocidade);
    }
}
sf::Texture* EntidadeColidivel::carregarTextura(const std::string caminho) const {
	sf::Texture* textura = new sf::Texture;
	if (!textura->loadFromFile(caminho)) {
        std::cout << "EntidadeColidivel::carregarTextura(string): ";
        std::cerr << "erro carregando ";
        std::cerr << caminho << std::endl;
    }
    return textura;
}
EntidadeColidivel* EntidadeColidivel::setPosicao(float x, float y) {
    setPosition(x,y);
    return (this);
}
EntidadeColidivel* EntidadeColidivel::setPosicao(const sf::Vector2f pos) {
    setPosition(pos);
    return (this);
}
sf::Vector2f EntidadeColidivel::getPosicao() const { 
    return getPosition();
}
sf::Vector2f EntidadeColidivel::getPosicaoDoCanto() const {
    float x = getPosition().x - getArea().x/2;
    float y = getPosition().y - getArea().y/2;
	return sf::Vector2f(x,y);
}

EntidadeColidivel* EntidadeColidivel::setSize(const sf::Vector2f tamanho) {
	_corpo.setSize(tamanho);
    return (this);
}
sf::Vector2f EntidadeColidivel::getSize() const {
	return _corpo.getSize();
}

EntidadeColidivel* EntidadeColidivel::setOrigem(float x,float y) {
    setOrigin(x,y);
    return (this);
}
EntidadeColidivel* EntidadeColidivel::setOrigem(const sf::Vector2f pos) {
    setOrigin(pos);
    return (this);
}
sf::Vector2f EntidadeColidivel::getOrigem() const {
	return getOrigin();
}

EntidadeColidivel* EntidadeColidivel::setTextura (const sf::Texture* textura) {
	_corpo.setTexture(textura);
    return (this);
}

EntidadeColidivel* EntidadeColidivel::setArea(sf::Vector2f area) {
	_area = area;
    return (this);
}
sf::Vector2f EntidadeColidivel::getArea() const {
	return _area;
}

EntidadeColidivel* EntidadeColidivel::setVelocidade(sf::Vector2f velocidade) {
	_velocidade = velocidade;
    return (this);
}
EntidadeColidivel* EntidadeColidivel::setVelocidade(float x, float y) {
	_velocidade = sf::Vector2f(x,y);
    return (this);
}
sf::Vector2f EntidadeColidivel::getVelocidade() const {
	return _velocidade;
}

EntidadeColidivel* EntidadeColidivel::setAceleracao(const sf::Vector2f aceleracao) {
	_aceleracao = aceleracao;
    return (this);
}
EntidadeColidivel* EntidadeColidivel::setAceleracao(float x, float y) {
	_aceleracao = sf::Vector2f(x,y);
    return (this);
}
sf::Vector2f EntidadeColidivel::getAceleracao() const {
	return _aceleracao;
}
EntidadeColidivel* EntidadeColidivel::setCoeficiente(float coeficiente) {
	_coeficiente = coeficiente;
    return (this);
}
float EntidadeColidivel::getCoeficiente() const {
	return _coeficiente;
}

EntidadeColidivel* EntidadeColidivel::setEstatico(bool estatico) {
    _estatico = estatico;
    return (this);
}
bool EntidadeColidivel::getEstatico() const {
    return _estatico;
}

EntidadeColidivel* EntidadeColidivel::setIdTipoObjeto(const int tipo) {
    _id_tipo_objeto = tipo;
    return (this);
}
int EntidadeColidivel::getIdTipoObjeto() const {
    return _id_tipo_objeto;
}

float EntidadeColidivel::distanciaEuclidiana(const EntidadeColidivel* const entidade) const {
	if (entidade != NULL) {
		float distancia_x = this->getPosition().x - entidade->getPosition().x;
		float distancia_y = this->getPosition().y - entidade->getPosition().y;
		return (std::sqrt((distancia_x*distancia_x) + (distancia_y*distancia_y)));
	}
	return FLT_MAX;
}
float EntidadeColidivel::distanciaEuclidiana(sf::Vector2f posicao) const {
    float distancia_x = this->getPosition().x - posicao.x;
    float distancia_y = this->getPosition().y - posicao.y;
    return (std::sqrt((distancia_x*distancia_x) + (distancia_y*distancia_y)));
}

void EntidadeColidivel::descartar() {
    _descartado = true;
}
bool EntidadeColidivel::isDescartado() const {
    return _descartado;
}

std::ofstream& EntidadeColidivel::operator >> (std::ofstream& out) const {
    out << getIdTipoObjeto()  << ' '
    << getPosicao()           << ' '
    << getVelocidade()        << ' ';
    return out;
} 
std::ifstream& EntidadeColidivel::operator << (std::ifstream& in) {
    sf::Vector2f posicao;
    sf::Vector2f velocidade;
    in >> posicao
    >> velocidade;
    setPosicao (posicao);
    setVelocidade (velocidade);
    return in;
}
