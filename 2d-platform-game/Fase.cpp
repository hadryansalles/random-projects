#include "Fase.hpp"
#include "Principal.hpp"
const float Fase::gravidade (0.1f);
float Fase::_cronometro (0.0f);

Fase::Fase (Janela* janela, sf::IntRect fim_fase) 
:   _painel_jogador_1("XXXXX", Principal::Altura/20)
,   _painel_jogador_2("XXXXX", Principal::Altura/20)
,   _numero_jogadores ( 0 )
,   _jogador_1 (NULL)
,   _jogador_2 (NULL)
,   _janela(janela)
,   _caixa_madeira (NULL)
,   _bloco_chao (NULL)
,   _fim_fase (fim_fase)
{
    sf::Vector2f pos_camera = _janela->getCameraPos();
    float pos_x = pos_camera.x - Principal::Largura/2.1;
    float pos_y = pos_camera.y - Principal::Altura/2.1;
    _painel_jogador_1.setPosition(pos_x, pos_y);
    _painel_jogador_2.setPosition(
        pos_x + _painel_jogador_1.getLargura(), pos_y);
}
Fase::~Fase() {
    // jogadores serao deletados na lista de entidade;
    if (_jogador_1) {
        _jogador_1 = NULL;
    }
    if (_jogador_2) {
        _jogador_2 = NULL;
    }
    if (_caixa_madeira) {
        delete _caixa_madeira;
    }
    if (_bloco_chao) {
        delete _bloco_chao;
    }
    _musica_de_fundo.stop();
}
void Fase::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_entidades, states);
    target.draw(_projeteis, states);
    if (_jogador_1) {
        target.draw(_painel_jogador_1);
    }
    if (_jogador_2) {
        target.draw(_painel_jogador_2);
    }
}

void Fase::atualizar() {
	moverEntidades();
    checaColisaoArtefatosAtaquePersonagens();
    checaColisaoEntidades();
    matarPersonagensSemVida();
    matarPersonagensQueSairamDoCenario();
    removerPersonagensMortos();
    descartarProjeteis();
    destruirEntidadesDescartadas();
	personagensAtacar();
    atualizaPainelPontuacao();
    definirJogadorPrincipal();
    reiniciarCronometro();
}

void Fase::moverEntidades() const {
    _entidades.mover();
    _projeteis.mover();
}

void Fase::checaColisaoArtefatosAtaquePersonagens() {
    _inimigos.checarColisoes(_artefatos_ataque);
    if (_jogador_1 != NULL) {
        _artefatos_ataque.checarColisao(_jogador_1);
    }
    if (_jogador_2 != NULL) {
        _artefatos_ataque.checarColisao(_jogador_2);
    }
}
void Fase::checaColisaoEntidades () {
    _entidades.moverPorColisoes();
}
void Fase::matarPersonagensSemVida() {
    _inimigos.matarSemVida();
    if (_jogador_1 != NULL && _jogador_1->getVida() <= 0 && _jogador_1->estaVivo()) {
        _jogador_1->matar();
    }
    if (_jogador_2 != NULL && _jogador_2->getVida() <= 0 && _jogador_2->estaVivo()) {
        _jogador_2->matar();
    }
}
void Fase::matarPersonagensQueSairamDoCenario() {
    _inimigos.matarSairamDoCenario(Principal::Altura);
    if (_jogador_1 != NULL 
        && _jogador_1->getPosicao().y + _jogador_1->getArea().y/2.0f > Principal::Altura
        && _jogador_1->estaVivo()) {
        _jogador_1->matar();
    }
    if (_jogador_2 != NULL
        && _jogador_2->getPosicao().y + _jogador_2->getArea().y/2.0f > Principal::Altura
        && _jogador_2->estaVivo()) {
        _jogador_2->matar();
    }
}
void Fase::removerPersonagensMortos() {
    _inimigos.removerMortos();
    if (_jogador_1 != NULL && _jogador_1->morreu()) {
        _numero_jogadores--;
        _jogador_1->descartar();
        _jogador_1 = NULL;
        Inimigo::setJogador1 (NULL);
        if (_jogador_2) {
            _janela->setJogadorPrincipal(_jogador_2);
        }
    }
    if (_jogador_2 != NULL && _jogador_2->morreu()) {
        _numero_jogadores--;
        _jogador_2->descartar();
        _jogador_2 = NULL;
        Inimigo::setJogador1 (NULL);
        if (_jogador_1) {
            _janela->setJogadorPrincipal(_jogador_1);
        }
    } 
}
void Fase::descartarProjeteis() {
    _projeteis.descartarDesativados();
    _projeteis.descartarSairamDoCenario(Principal::Altura);
}
void Fase::destruirEntidadesDescartadas() {
    _entidades.destruirDescartados();
    _artefatos_ataque.destruirDescartados();
    _projeteis.removerDescartados();
}
void Fase::personagensAtacar() {
    _inimigos.atacar(_projeteis, _artefatos_ataque);
    if (_jogador_1 != NULL) {
        _jogador_1->atacar();
    }
    if (_jogador_2 != NULL) {
        _jogador_2->atacar();
    }
}
void Fase::atualizaPainelPontuacao() {
    sf::Vector2f pos_camera = _janela->getCameraPos();
    if (_jogador_1 && _jogador_2) {
        _painel_jogador_1.setPosition(
            pos_camera.x - 150,
            pos_camera.y - Principal::Altura / 2.1);
        _painel_jogador_2.setPosition(
            pos_camera.x - 120 + _painel_jogador_1.getLargura(),
            pos_camera.y - Principal::Altura / 2.1);
        _painel_jogador_1.atualizaPontuacao(_jogador_1->getPontuacao());
        _painel_jogador_2.atualizaPontuacao(_jogador_2->getPontuacao());
    }
    else if (_jogador_1) {
        _painel_jogador_1.setPosition(
            pos_camera.x - 150,
            pos_camera.y - Principal::Altura / 2.1);
        _painel_jogador_1.atualizaPontuacao(_jogador_1->getPontuacao());
    }
    else if (_jogador_2) {
        _painel_jogador_2.setPosition(
            pos_camera.x - Principal::Largura / 2.1,
            pos_camera.y - Principal::Altura / 2.1);
        _painel_jogador_2.atualizaPontuacao(_jogador_2->getPontuacao());
    }
}

void Fase::definirJogadorPrincipal() {
    if (_jogador_2 != NULL && _jogador_1 != NULL) {
        if (_jogador_1->getPosicao().x > _jogador_2->getPosicao().x) {
            _janela->setJogadorPrincipal(_jogador_1);
        }
        else {
            _janela->setJogadorPrincipal(_jogador_2);
        }
    }
}

void Fase::reiniciarCronometro() {
    // salva o tempo que demorou para executar
    // o frame anterior em segundos
	_cronometro = _clock.restart().asSeconds();
}

void Fase::continuarJogo() {
    _musica_de_fundo.play();
}

std::pair<bool,bool> Fase::getExistenciaJogadores() const {
    std::pair<bool,bool> existe;
    existe.first = (_jogador_1 != NULL ? true : false); 
    existe.second = (_jogador_2 != NULL ? true : false);
    return existe;
}

int Fase::getEstado() {
    if (_numero_jogadores <= 0) {
        _musica_de_fundo.pause();
        return Codigos::GAME_OVER;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
        _musica_de_fundo.pause();
        return Codigos::PAUSE;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
        _musica_de_fundo.pause();
        return Codigos::PAUSE;
    }
    // terminou a fase
    if (_jogador_1 != NULL) {
        sf::Vector2f pos = _jogador_1->getPosicao(); 
        if (pos.x > _fim_fase.left && pos.x < _fim_fase.left + _fim_fase.width
         && pos.y > _fim_fase.top && pos.y < _fim_fase.top + _fim_fase.height) {
            salvarPontuacao();
            _musica_de_fundo.pause();
            return Codigos::FIM_FASE;
        }
    }
    if (_jogador_2 != NULL) {
        sf::Vector2f pos = _jogador_2->getPosicao(); 
        if (pos.x > _fim_fase.left && pos.x < _fim_fase.left + _fim_fase.width
         && pos.y > _fim_fase.top && pos.y < _fim_fase.top + _fim_fase.height) {
            salvarPontuacao();
            _musica_de_fundo.pause();
            return Codigos::FIM_FASE;
        }
    }
    return Codigos::CONTINUE;
} 

void Fase::adicionar(ArtefatoAtaque* ataque) {
    if (ataque) {
        _artefatos_ataque.push_back(ataque);
    }
}

void Fase::remover(ArtefatoAtaque* ataque) {
    _artefatos_ataque.remove(ataque);
}

void Fase::adicionar(Projetil* projetil) {
    if (projetil) {
        _projeteis.push_back(projetil);
    }
}

void Fase::remover(Projetil* projetil) {
    _projeteis.remove(projetil);
}
void Fase::adicionar(EntidadeColidivel* entidade) {
    if (entidade) {
        _entidades.push_back(entidade);
    }
}
void Fase::remover(EntidadeColidivel* entidade) {
    _entidades.remove(entidade);
}
void Fase::adicionar(Inimigo* inimigo) {
    if (inimigo) {
        _inimigos.push_front(inimigo);
    }
}
void Fase::remover(Inimigo* inimigo) {
	_inimigos.remove(inimigo);
}

sf::Vector2f Fase::getArea() const {
	return _area_da_fase;
}

float Fase::getCronometro(){
    return _cronometro;
}

sf::Texture Fase::carregarTextura(std::string caminho) {
    sf::Texture textura;
	if (!textura.loadFromFile(caminho)) {
        std::cerr << "Fase::carregarTextura(string): ";
        std::cerr << "erro carregando ";
        std::cerr << caminho << std::endl;
    }
    return (textura);
}

void Fase::limparListas() {
    _artefatos_ataque.limparLista();
    _entidades.limparLista();
    _inimigos.clear();
}

void Fase::iniciarJogadores(bool jogador1, bool jogador2) { 
    if (jogador1 && _jogador_1 == NULL) {
        _jogador_1 = new Jogador(Codigos::JOGADOR_1);
        Inimigo::setJogador1(_jogador_1);
		adicionar(_jogador_1->getArtefatoAtaque());
        _janela->setJogadorPrincipal(_jogador_1);
        _numero_jogadores++;
        adicionar (static_cast<EntidadeColidivel*> (_jogador_1));
    }
    if (jogador2 && _jogador_2 == NULL) {
        _jogador_2 = new Jogador(Codigos::JOGADOR_2);
        Inimigo::setJogador2(_jogador_2);
		adicionar(_jogador_2->getArtefatoAtaque());
        _janela->setJogadorPrincipal(_jogador_2);
        _numero_jogadores++;
        adicionar (static_cast<EntidadeColidivel*> (_jogador_2));
    }
}

void Fase::criarParedes () {
    EntidadeColidivel* parede_1 = new EntidadeColidivel;
    parede_1->setPosicao(sf::Vector2f(_area_da_fase.x - 50.0f, 0.0f));
    parede_1->setArea (sf::Vector2f(100.0f, Principal::Altura*2.0f));
    parede_1->setEstatico(true);
    adicionar (parede_1);

    EntidadeColidivel* parede_2 = new EntidadeColidivel;
    parede_2->setPosicao(sf::Vector2f(_area_da_fase.y + 50.0f, 0.0f));
    parede_2->setArea (sf::Vector2f(100.0f, Principal::Altura*2.0f));
    parede_2->setEstatico(true);
    adicionar (parede_2);

    EntidadeColidivel* parede_3 = new EntidadeColidivel;
    parede_3->setPosicao(sf::Vector2f(_area_da_fase.y/2.0f, -50.0f));
    parede_3->setArea (sf::Vector2f(_area_da_fase.y, 100.0f));
    parede_3->setEstatico(true);
    adicionar (parede_3);
}

void Fase::setMusica(std::string caminho, float volume) { 
    if (!_musica_de_fundo.openFromFile(caminho)) {
        std::cerr << "Fase::setMusica: "
        << "Erro ao carregar musica da fase "
        << caminho << std::endl;
    }
    _musica_de_fundo.setVolume(volume);
    _musica_de_fundo.setLoop(true);
}

bool Fase::temJogadorProximo(sf::Vector2f posicao) const {
    if (_jogador_1 != NULL &&
        _jogador_1->getPosicao().x > posicao.x - Principal::Largura * 0.4f) {
        return true;
    }
    if (_jogador_2 != NULL &&
        _jogador_2->getPosicao().x > posicao.x - Principal::Largura * 0.4f) {
        return true;
    }
    return false;
} 
EntidadeColidivel* Fase::criarCaixaMadeira(float pos_x, float pos_y) {
    EntidadeColidivel* caixa = new EntidadeColidivel(Codigos::CAIXA_MADEIRA);
    if ( _caixa_madeira == NULL ) {
        _caixa_madeira = caixa->carregarTextura ("sprites/caixa.png");
    }
    caixa
        ->setTextura ( _caixa_madeira )
        ->setIdTipoObjeto ( Codigos::CAIXA_MADEIRA )
        ->setPosicao(pos_x, pos_y)
        ->setCoeficiente(0.8f)
        ->setEstatico(false)
        ->setOrigem((float)_caixa_madeira->getSize().x / 2, (float)_caixa_madeira->getSize().y / 2)
        ->setArea(sf::Vector2f((float)_caixa_madeira->getSize().x, (float)_caixa_madeira->getSize().y))
        ->setSize((sf::Vector2f)_caixa_madeira->getSize());
    adicionar (static_cast<EntidadeColidivel*> (caixa));
    return ( caixa );
}
EsqueletoNu* Fase::criarEsqueletoNu(sf::Vector2f posicao) {
    auto novo_esqueleto = new EsqueletoNu(posicao);
    adicionar (novo_esqueleto->getArtefatoAtaque());
    adicionar (static_cast <EntidadeColidivel*> (novo_esqueleto));
    adicionar (static_cast <Inimigo*> (novo_esqueleto));
    return novo_esqueleto;
}
EsqueletoRoupa* Fase::criarEsqueletoRoupa(sf::Vector2f posicao) {
    auto novo_esqueleto = new EsqueletoRoupa(posicao);
    adicionar (novo_esqueleto->getArtefatoAtaque());
    adicionar (static_cast <EntidadeColidivel*> (novo_esqueleto));
    adicionar (static_cast <Inimigo*> (novo_esqueleto));
    return novo_esqueleto;
}
void Fase::criarChao(std::string caminho) {
//    primeiro chao em x = 0 para pegar a largura de cada bloco
    // O chao sera criado pouco antes da borda da janela 
    EntidadeColidivel* chao = criarBlocoChao ( 0.0f , Principal::Altura * 0.925f, 0.6f, caminho);
    float largura_chao = chao->getSize().x;
    for (int i = 1; i <= 1 + _area_da_fase.y / largura_chao; i++) {
        criarBlocoChao (i * largura_chao, Principal::Altura * 0.925f, 0.6f, caminho);
        i++;
    }
}
EntidadeColidivel* Fase::criarBlocoChao(float pos_x, float pos_y, float escala, std::string caminho) {
    EntidadeColidivel* chao = new EntidadeColidivel(Codigos::CHAO);
    if ( _bloco_chao == NULL ) {
        _bloco_chao = chao->carregarTextura (caminho);
    }
    chao
        ->setTextura ( _bloco_chao )
        ->setIdTipoObjeto ( Codigos::CHAO )
        ->setCoeficiente(1.0f)
        ->setEstatico(true)
        ->setOrigem((float)_bloco_chao->getSize().x / 2, (float)_bloco_chao->getSize().y / 2)
        ->setArea(sf::Vector2f((float)_bloco_chao->getSize().x, (float)_bloco_chao->getSize().y * escala))
        ->setSize(sf::Vector2f((float)_bloco_chao->getSize().x, (float)_bloco_chao->getSize().y))
        ->setPosicao(pos_x, pos_y);
    adicionar (static_cast<EntidadeColidivel*> (chao));
    return ( chao );
}

Fase::PainelPontuacao::PainelPontuacao(std::string nome_jogador, unsigned int tamanho) {
    std::string caminho_fonte = "fonts/Ancient.ttf";
    if (!_fonte.loadFromFile(caminho_fonte)) {
        std::cerr << "PainelPontuacao::PainelPontuacao: "
        << "Erro ao carregar fonte "
        << caminho_fonte << std::endl;
    }
    _nome_jogador.setFont(_fonte);
    _nome_jogador.setCharacterSize(tamanho);
    _nome_jogador.setFillColor(sf::Color(255,255,255));
    _nome_jogador.setString(nome_jogador);

    _pontuacao.setFont(_fonte);
    _pontuacao.setCharacterSize(tamanho);
    _pontuacao.setFillColor(sf::Color(255,255,255));
    _pontuacao.setString("0.0");
    // em baixo do nome
    _pontuacao.setPosition(0.0f, _nome_jogador.getGlobalBounds().height);
    atualizaPontuacao(0.0f);
}

Fase::PainelPontuacao::~PainelPontuacao() {
}

void Fase::PainelPontuacao::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(_nome_jogador, states);
    target.draw(_pontuacao, states);
}

float Fase::PainelPontuacao::getLargura() const {
    return _nome_jogador.getGlobalBounds().width;
}

void Fase::PainelPontuacao::atualizaPontuacao(float pontuacao) {
    std::string str = std::to_string(pontuacao);
    str.erase (str.end() - 4, str.end());
    _pontuacao.setString(str);
    sf::FloatRect nome = _nome_jogador.getGlobalBounds();
    sf::FloatRect pont = _pontuacao.getGlobalBounds();
    float pos_x = nome.left + nome.width - pont.width;
    _pontuacao.setPosition(pos_x, _pontuacao.getPosition().y);
}

void Fase::PainelPontuacao::setNomeJogador(std::string nome) {
    _nome_jogador.setString (nome);
}

std::ofstream& Fase::PainelPontuacao::operator >> (std::ofstream& out) {
    out << _nome_jogador.getString() << '|'
    << getPosition().x               << '|'
    << getPosition().y               << '|';
    return out;
}

std::ifstream& Fase::PainelPontuacao::operator << (std::ifstream& in) {
    sf::Vector2f posicao;
    std::string nome, pos;
    getline(in, nome, '|');
    getline(in,  pos, '|');
    posicao.x = std::stof(pos);
    getline(in,  pos, '|');
    posicao.y = std::stof(pos);
    setPosition (posicao);
    _nome_jogador.setString (nome);
    return in;
} 

void Fase::salvarPontuacao() const {
    std::ofstream out ("saves/Ranking.data", std::ios::app);
    if (_jogador_1) {
        out << _jogador_1->getPontuacao() << '|'
        << _jogador_1->getNome() << '|' << '\n';
    }
    if (_jogador_2) {
        out << _jogador_2->getPontuacao() << '|'
        << _jogador_2->getNome() << '|' << '\n';
    }
} 
