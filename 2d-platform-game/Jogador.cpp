#include "Std.hpp"
#include "Jogador.hpp"
#include "Fase.hpp"

int Jogador::quantidade ( 0 );
const float Jogador::VidaInicial ( 25.0f );
const float Jogador::VelocidadeMaxima ( 2.0f );

Jogador::Jogador(int tipo)
:   Personagem(VidaInicial, true, true, 1000.0f)
,	_esta_atacando (false)
,	_esta_tomando_dano (false)
,	_esta_imune (false)
,   _esta_pulando (false)
,	_tempo_imune ( 0.0f )
,   _pontuacao (0.0f)
,   _nome ("Jogador") {
    setIdTipoObjeto (tipo);
    _espada = new ArtefatoAtaque (4.0f, 0.12f,
                sf::Vector2f(30.0f, 5), this);
	setDoBem(true);
    setCoeficiente ( 0.8f );
	setVelocidade(sf::Vector2f(0.0f, 0.0f));
	setArea(sf::Vector2f(28,38));

    _som_espada_buffer.loadFromFile("sounds/espada_1.wav");
    _som_espada.setBuffer(_som_espada_buffer);
    _som_pulo_buffer.loadFromFile("sounds/vento.wav");
    _som_pulo.setBuffer(_som_pulo_buffer);
    setSomDano("sounds/dano.wav");

    _barra_de_vida.setConfiguracoes(getPosicaoDoCanto(), getArea().x, getVida() * 2);
	carregarComandos();
	carregarTexturas();

    if (tipo == Codigos::JOGADOR_2) {
        _corpo.setFillColor(sf::Color(128,255,255,255));
        _barra_de_vida.setColor(sf::Color(128,255,255,255));
    }

	p_animacao_atual = &_parado;
	p_animacao_atual->atualiza(_direita, _corpo, 0.2f);
}
Jogador::~Jogador() {
    _espada->descartar();
}

void Jogador::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    EntidadeColidivel::draw(target,states);
    _barra_de_vida.draw(target,states);
}

void Jogador::carregarTexturas() {
	_parado.setConfig("sprites/jogador-parado.png",
		4, 0.2f);
	_correndo.setConfig("sprites/jogador-correndo.png",
		12, 0.08f);
	_atacando.setConfig("sprites/jogador-atacando.png",
		6, 0.08f);
	_tomando_dano.setConfig("sprites/jogador-dano.png",
		3, 0.08f);
	_pulando_parado.setConfig("sprites/jogador-pulando-parado.png",
		4, 0.08f);
	_pulando_correndo.setConfig("sprites/jogador-pulando-correndo.png",
		4, 0.15f);
}

void Jogador::carregarComandos() {
    if (getIdTipoObjeto() == Codigos::JOGADOR_1) {
        _esquerda = sf::Keyboard::Key::A;
        _direita = sf::Keyboard::Key::D;
        _cima = sf::Keyboard::Key::W;
        _baixo = sf::Keyboard::Key::S;
        _t_ataque1 = sf::Keyboard::Key::Q;
    }
    else {
        _esquerda = sf::Keyboard::Key::J;
        _direita = sf::Keyboard::Key::L;
        _cima = sf::Keyboard::Key::I;
        _baixo = sf::Keyboard::Key::K;
        _t_ataque1 = sf::Keyboard::Key::U;
    }
}
void Jogador::mover() {
    bool movendo = false;
	if (!_esta_atacando && estaVivo() && !_esta_tomando_dano && !estaEmAnimacaoDePulo()) {
		if (sf::Keyboard::isKeyPressed(_direita)) {
			movendo = true;
			setEstaParaDireita(true);
			p_animacao_atual = &_correndo;

			// Melhora o controle do personagem no ar
			if (getVelocidade().x < VelocidadeMaxima && _esta_pulando) {
				setVelocidade(getVelocidade().x + 0.4f, getVelocidade().y);
			}
			else {
				setVelocidade(VelocidadeMaxima, getVelocidade().y);
			}
		}
		else if (sf::Keyboard::isKeyPressed(_esquerda)) {
			movendo = true;
			setEstaParaDireita(false);
			p_animacao_atual = &_correndo;
			if (getVelocidade().x > -VelocidadeMaxima && _esta_pulando) {
				setVelocidade(getVelocidade().x - 0.4f, getVelocidade().y);
			}
			else {
				setVelocidade(-VelocidadeMaxima, getVelocidade().y);
			}
		}
		if ((sf::Keyboard::isKeyPressed(_baixo) ||
            sf::Keyboard::isKeyPressed(_cima)) && !_esta_pulando) {
			if (movendo == true) {
				_pulando_correndo.reiniciar();
				p_animacao_atual = &_pulando_correndo;
			}
			else {
				_pulando_parado.reiniciar();
				p_animacao_atual = &_pulando_parado;
			}
			movendo = true;
			_esta_pulando = true;
			_som_pulo.play();
			setVelocidade(getVelocidade().x, -3.51f);
		}
	}

    if ( ! movendo && !estaEmAnimacaoDePulo()) {
        p_animacao_atual = &_parado;
    }

    if ( getVelocidade().y == 0 ) {
        _esta_pulando = false;
    }
	else { 
		// caso a velocidade seja diferente de 0, jogador esta no ar
		// para nao haver pulos duplos
		_esta_pulando = true;
	}

	p_animacao_atual->atualiza(estaParaDireita(), _corpo);
    EntidadeColidivel::mover();
    _barra_de_vida.atualizar(2 * getVida(), getPosicao());
}
Projetil* Jogador::atacar() {
	_tempo_imune += Fase::getCronometro();
	if (_tempo_imune > 0.1f) {
		_tempo_imune = 0;
		_esta_imune = false;
	}
	if (_para_direita) {
		_espada->setPosicao(getPosicao().x + getArea().x, getPosicao().y);
	}
	else {
		_espada->setPosicao(getPosicao().x - getArea().x, getPosicao().y);
	}
	if (estaVivo() && !_esta_tomando_dano && !estaEmAnimacaoDePulo()) {
		if (!_esta_atacando) {
			if (sf::Keyboard::isKeyPressed(_t_ataque1)) {
				_esta_atacando = true;
                _som_espada.play();
				_atacando.reiniciar();
			}
		}
		else {
			if (_atacando.executou(3) && _espada->recarregou()) {
                _espada->setTempoUltimoArtefatoAtaque(0.0f);
                _espada->ativar();
			}
			if (!_atacando.terminou()) {
				_atacando.atualiza(estaParaDireita(), _corpo);
			}
			else {
				_atacando.zerar();
				_parado.atualiza(estaParaDireita(), _corpo);
				_espada->desativar();
				_esta_atacando = false;
			}
		}
	}
	if (_esta_tomando_dano) {
		_tomando_dano.atualiza( estaParaDireita(), _corpo);
		if (_tomando_dano.terminou()) {
			_esta_imune = true;
			_tempo_imune = 0;
			_esta_tomando_dano = false;
		}
	}
	return NULL;
}
void Jogador::diminuirVidaEm(float dano) {
	if (!_esta_imune && !_esta_tomando_dano) {
		Personagem::diminuirVidaEm(dano);
		_esta_imune = true;
		_esta_tomando_dano = true;
		_tomando_dano.reiniciar();
		_tomando_dano.atualiza(estaParaDireita(), _corpo);
    }
}
ArtefatoAtaque* Jogador::getArtefatoAtaque() const {
	return _espada;
}
std::string Jogador::getNome() const {
	return _nome;
}
void Jogador::setNome(std::string nome) {
	_nome = nome;
}
Jogador::BarraDeVida::BarraDeVida() {
}
void Jogador::BarraDeVida::setConfiguracoes(sf::Vector2f pos, float largura, int n_divisoes) {
    _barra_de_vida.setPrimitiveType(sf::Quads);
    _n_divisoes = n_divisoes;
    pos = sf::Vector2f(pos.x ,pos.y -5);
    sf::Vector2f tam = sf::Vector2f(largura , 3);
    _barra_de_vida.resize(n_divisoes * 4);
    float tam_div = tam.x / (float) n_divisoes;

    for (int i = 0; i < n_divisoes; i++) {
        _barra_de_vida[ i * 4 ].position = sf::Vector2f((tam_div * i) + pos.x, pos.y);
        _barra_de_vida[i*4 + 1].position = sf::Vector2f((tam_div * (i + 1)) + pos.x, pos.y);
        _barra_de_vida[i*4 + 2].position = sf::Vector2f((tam_div * (i + 1)) + pos.x, pos.y + tam.y);
        _barra_de_vida[i*4 + 3].position = sf::Vector2f((tam_div * i) + pos.x, pos.y + tam.y);

        _barra_de_vida[ i * 4 ].texCoords = sf::Vector2f(0.f, 0.f);
        _barra_de_vida[i*4 + 1].texCoords = sf::Vector2f(tam.x, 0.f);
        _barra_de_vida[i*4 + 2].texCoords = sf::Vector2f(tam.x, tam.y);
        _barra_de_vida[i*4 + 3].texCoords = sf::Vector2f(0.f, tam.y);
    }
    setColor(sf::Color(255,0,0,255));
}
Jogador::BarraDeVida::~BarraDeVida() {
}
void Jogador::BarraDeVida::setColor (const sf::Color& cor) {
    for (std::size_t i = 0; i < _barra_de_vida.getVertexCount(); i++) {
        _barra_de_vida[ i ].color = cor;
    }
}
void Jogador::BarraDeVida::atualizar(float vida, sf::Vector2f pos) {
    setPosition (pos);
    for (int i = vida; i < _n_divisoes; i++) {
        for (int j = 0; j < 4; j++) {
            _barra_de_vida[ i * 4 + j].color = sf::Color(200,255,255,100);
        }
    }
}
void Jogador::BarraDeVida::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_barra_de_vida,states);
}

std::ofstream& Jogador::operator >> (std::ofstream& out) {
    out << getIdTipoObjeto()  << ' '
    << _vida                  << ' '
    << getPosicao()           << ' '
    << getVelocidade()        << ' '
    << getPontuacao()         << ' '
    << (estaParaDireita() ? 1 : 0);
    out << '|' << _nome << '|';
    return out;
} 
std::ifstream& Jogador::operator << (std::ifstream& in) {
    float vida = 0;
    sf::Vector2f posicao;
    sf::Vector2f velocidade;
    float pontuacao;
    int estaParaDireita = 0;
    std::string nome;

    in >> vida
    >> posicao
    >> velocidade
    >> pontuacao
    >> estaParaDireita;
    std::getline(in, nome, '|');
    std::getline(in, nome, '|');
    _vida = float (vida);
    _pontuacao = pontuacao;
    setPosicao (posicao);
    setVelocidade (velocidade);
    setEstaParaDireita(estaParaDireita == 0 ? false : true);
    return in;
} 

bool Jogador::estaEmAnimacaoDePulo() {
	if (_pulando_parado.estaAtiva()) {
		return true;
	}
	else if (_pulando_correndo.estaAtiva()) {
		return true;
	}
	return false;
}
float Jogador::getPontuacao() const {
    return _pontuacao;
}
void Jogador::alterarPontuacao(float delta) {
    _pontuacao += delta;
}
