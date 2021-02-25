#include "Fase2.hpp"
#include "Principal.hpp"

const int Fase2::AlturaChao (Principal::Altura * 0.925);
const sf::Vector2f Fase2::SpawnJogador1 (100, AlturaChao * 0.9);
const sf::Vector2f Fase2::SpawnJogador2 ( 70, AlturaChao * 0.9);

Fase2::Fase2 (std::string jogador1, std::string jogador2, Janela* janela)
:   Fase (janela, sf::IntRect (1870, 198, 44, 62))
,   _espinho (NULL)
{
    // imagens de fundo o cenario
	carregarCenario();

    spawnarCaixasMadeira();

    // chao da fase inteira
    criarChao("sprites/chao2_att.png");

    // paredes nos limetes da fase
    criarParedes();

    // escolhe as posicoes das entidades na fase    
    sorteiaPosicaoEntidades();

    iniciarJogadores (!jogador1.empty(), !jogador2.empty());

	// seleciona onde os jogadores iram iniciar na fase
	if (_jogador_1 != NULL) {
        _painel_jogador_1.setNomeJogador(jogador1); 
        _jogador_1->setNome(jogador1);
		_jogador_1->setPosicao(SpawnJogador1);
	}
	if (_jogador_2 != NULL) {
        _painel_jogador_2.setNomeJogador(jogador2); 
        _jogador_2->setNome(jogador2);
		_jogador_2->setPosicao(SpawnJogador2);
	}
    setMusica("sounds/forward_assault.wav", 50.0f);
    _musica_de_fundo.play();
}

Fase2::Fase2 (Janela* janela)
:   Fase (janela, sf::IntRect (1870, 198, 44, 62))
,   _espinho (NULL) {
	carregarCenario();
    criarParedes();
    criarChao("sprites/chao2_att.png");
    carregar();
    setMusica("sounds/forward_assault.wav", 50.0f);
    _janela->atualizarCamera();
    _musica_de_fundo.play();
}

Fase2::~Fase2() {
}

void Fase2::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw (_cenario_dinamico, states);
	target.draw (_portal_fim_fase, states);
    Fase::draw (target,states);
}

void Fase2::atualizar() {
    spawnarInimigos();
    spawnarEspinhos();
    spawnarCaixasMadeira();
    Fase::atualizar();
}

void Fase2::spawnarInimigos() {
    spawnarEsqueletosNu();
    spawnarEsqueletosRoupa();
    spawnarFantasmas();
    spawnarLadroes();
}

void Fase2::spawnarEspinhos() {
    std::vector<sf::Vector2f>::iterator pos = _pos_espinhos.begin();
    while (pos != _pos_espinhos.end()) {
       if (temJogadorProximo(*pos)) {
            criarEspinho (*pos);
            pos = _pos_espinhos.erase(pos);
        }
        else pos++;
    }
}

void Fase2::spawnarFantasmas() {
    std::vector<sf::Vector2f>::iterator pos = _pos_fantasmas.begin();
    while (pos != _pos_fantasmas.end()) {
       if (temJogadorProximo(*pos)) {
            criarFantasma (*pos);
            pos = _pos_fantasmas.erase(pos);
        }
        else pos++;
    }
}

void Fase2::spawnarLadroes() {
    std::vector<sf::Vector2f>::iterator pos = _pos_ladroes.begin();
    while (pos != _pos_ladroes.end()) {
       if (temJogadorProximo(*pos)) {
            criarLadrao (*pos);
            pos = _pos_ladroes.erase(pos);
        }
        else pos++;
    }
}

void Fase2::spawnarEsqueletosRoupa() {
    std::vector<sf::Vector2f>::iterator pos = _pos_esqueletos_roupa.begin();
    while (pos != _pos_esqueletos_roupa.end()) {
       if (temJogadorProximo(*pos)) {
            criarEsqueletoRoupa (*pos);
            pos = _pos_esqueletos_roupa.erase(pos);
        }
        else pos++;
    }
}

void Fase2::spawnarEsqueletosNu() {
    std::vector<sf::Vector2f>::iterator pos = _pos_esqueletos_nu.begin();
    while (pos != _pos_esqueletos_nu.end()) {
       if (temJogadorProximo(*pos)) {
            criarEsqueletoNu (*pos);
            pos = _pos_esqueletos_nu.erase(pos);
        }
        else pos++;
    }
}

void Fase2::spawnarCaixasMadeira() {
    std::vector<sf::Vector2f>::iterator pos = _pos_caixas.begin();
    while (pos != _pos_caixas.end()) {
       if (temJogadorProximo(*pos)) {
            criarCaixaMadeira ((*pos).x, (*pos).y);
            pos = _pos_caixas.erase(pos);
        }
        else pos++;
    }
}

void Fase2::sorteiaPosicaoEntidades() {
    srand(time(NULL));
    int i = 0;
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_esqueletos_nu.push_back (sf::Vector2f(200 + i, AlturaChao * 0.9));
    } while (( i % 51 || _pos_esqueletos_nu.size() < 5) && _pos_esqueletos_nu.size() < 6);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_esqueletos_roupa.push_back (sf::Vector2f(200 + i , AlturaChao * 0.9));
    } while (( i % 51 || _pos_esqueletos_roupa.size() < 5) && _pos_esqueletos_roupa.size() < 6);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_fantasmas.push_back (sf::Vector2f( 200 + i, Principal::Altura / 2));
    } while (( i % 51 || _pos_fantasmas.size() < 5) && _pos_fantasmas.size() < 6);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_ladroes.push_back (sf::Vector2f(200 + i, Principal::Altura * 0.9));
    } while (( i % 51 || _pos_ladroes.size() < 5) && _pos_ladroes.size() < 6);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_espinhos.push_back (sf::Vector2f( 200 + i, Principal::Altura * 0.88));
    } while (( i % 51 || _pos_espinhos.size() < 5) && _pos_espinhos.size() < 6);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_caixas.push_back (sf::Vector2f( 200 + i, Principal::Altura * 0.88));
    } while (( i % 51 || _pos_caixas.size() < 5) && _pos_caixas.size() < 6);
}

void Fase2::carregarCenario() {
    std::string s_cenario_dinamico ("sprites/fundo2.png");
	_t_cenario_dinamico = carregarTextura(s_cenario_dinamico);
	_cenario_dinamico.setTexture(&_t_cenario_dinamico);
	_cenario_dinamico.setSize(sf::Vector2f(_t_cenario_dinamico.getSize()));
	_cenario_dinamico.setOrigin(0, 0);
	_cenario_dinamico.setPosition(0, 0);

	//  os limites terminam no fim do cenario
	_area_da_fase.x = 0;
	_area_da_fase.y = _t_cenario_dinamico.getSize().x;

    std::string s_portal_fim_fase ("sprites/portal.png");
	_t_portal_fim_fase = carregarTextura(s_portal_fim_fase);
	_portal_fim_fase.setTexture(&_t_portal_fim_fase);
	_portal_fim_fase.setSize(sf::Vector2f(_t_portal_fim_fase.getSize()) * 0.4f);
	_portal_fim_fase.setOrigin(_portal_fim_fase.getSize() / 2.0f);
	_portal_fim_fase.setPosition(_area_da_fase.y - 30, 230);
    _portal_fim_fase.setFillColor(sf::Color(0,255,0,100));
}

Fantasma* Fase2::criarFantasma(sf::Vector2f posicao) {
    Fantasma* fantasma = new Fantasma (posicao);
    adicionar (fantasma->getArtefatoAtaque());
    adicionar (static_cast <EntidadeColidivel*> (fantasma));
    adicionar (static_cast <Inimigo*> (fantasma));
    return fantasma;
}

Ladrao* Fase2::criarLadrao(sf::Vector2f posicao) {
    Ladrao* ladrao = new Ladrao (posicao);
    adicionar (ladrao->getArtefatoAtaque());
    adicionar (static_cast <EntidadeColidivel*> (ladrao));
    adicionar (static_cast <Inimigo*> (ladrao));
    return ladrao;
}

Projetil* Fase2::criarEspinho(sf::Vector2f posicao) {
    Projetil* espinho = new Projetil(5, 0.2f,sf::Vector2f(28,8), NULL);
    if ( _espinho == NULL ) {
        _espinho = espinho->carregarTextura ("sprites/espinho.png");
    }
    espinho->setAnimador("sprites/espinho.png", 1, 10000);
    espinho->setDescartavel(false);
    espinho
        ->setIdTipoObjeto ( Codigos::ESPINHO )
        ->setPosicao(posicao)
        ->setCoeficiente(1.0f)
        ->setEstatico(true)
        ->setArea(sf::Vector2f((float)_espinho->getSize().x, (float)_espinho->getSize().y))
        ->setSize((sf::Vector2f)_espinho->getSize());
    adicionar (static_cast<Projetil*> (espinho));
    adicionar (static_cast<ArtefatoAtaque*> (espinho));
    return ( espinho );
}

void Fase2::salvar() {
    std::ofstream out_fase2 ("saves/fase2.data", std::ios::out);
    if (!out_fase2) {
        std::cerr << "Fase2::salvar ";
        std::cerr << "Gravador não pode ser aberto" << std::endl;
    }
    _painel_jogador_1 >> out_fase2;
    _painel_jogador_2 >> out_fase2;
    out_fase2 << "\nJOGADORES\n";
    if (_jogador_1) {
        (*_jogador_1) >> out_fase2 << '\n';
    }
    if (_jogador_2) {
        (*_jogador_2) >> out_fase2 << '\n';
    }
    out_fase2 << Codigos::FIM_SAVE << " FIM\n";
    
    out_fase2 << "LISTA_ENTIDADES\n";
    _entidades >> out_fase2;
    out_fase2 << Codigos::FIM_SAVE << " FIM\n";

    out_fase2 << "LISTA_PROJETEIS\n";
    _projeteis >> out_fase2;
    out_fase2 << Codigos::FIM_SAVE << " FIM\n";

    out_fase2 << "LISTA_INIMIGOS\n";
    _inimigos >> out_fase2;
    out_fase2 << Codigos::FIM_SAVE << " FIM\n";

    out_fase2 << "POSICAO_NAO_SPAWNADOS\n";
    std::vector<sf::Vector2f>::iterator it = _pos_esqueletos_roupa.begin();
    for (; it != _pos_esqueletos_roupa.end(); it++) {
        out_fase2 <<Codigos::POS_ESQUELETO_ROUPA << ' ' << (*it) << '\n';
    }
    it = _pos_esqueletos_nu.begin();
    for (; it != _pos_esqueletos_nu.end(); it++) {
        out_fase2 << Codigos::POS_ESQUELETO_NU << ' ' << (*it) << '\n';
    }
    it = _pos_fantasmas.begin();
    for (; it != _pos_fantasmas.end(); it++) {
        out_fase2 << Codigos::POS_FANTASMA << ' ' << (*it) << '\n';
    }
    it = _pos_ladroes.begin();
    for (; it != _pos_ladroes.end(); it++) {
        out_fase2 << Codigos::POS_LADRAO << ' ' << (*it) << '\n';
    }
    it = _pos_caixas.begin();
    for (; it != _pos_caixas.end(); it++) {
        out_fase2 << Codigos::POS_CAIXA_MADEIRA << ' ' << (*it) << '\n';
    }
    it = _pos_espinhos.begin();
    for (; it != _pos_espinhos.end(); it++) {
        out_fase2 << Codigos::POS_ESPINHO << ' ' << (*it) << '\n';
    }
    out_fase2 << Codigos::FIM_SAVE << " FIM\n";
    out_fase2 << "FIM_ARQUIVO\n";
    out_fase2.close();
}

bool Fase2::carregar() {
    std::ifstream in ("saves/fase2.data", std::ios::in);
    if (! in) {
        std::cerr << "Fase2::carregar: "
        << "nenhum arquivo foi encontrado"
        << "saves/arquivo.data";
        return false;
    }
    _painel_jogador_1 << in;
    _painel_jogador_2 << in;
    int tipo = 0;
    std::string linha;
    while (in.good()) {
        getline(in,linha);
        tipo = 0;
        if (linha.compare( 0, 9, "JOGADORES") == 0) {
            in >> tipo;
            if (tipo == Codigos::JOGADOR_1) {
                iniciarJogadores (true, false);
                if (_jogador_1) // alocou sem problemas
                    (*_jogador_1) << in;
                in >> tipo;
            }
            if (tipo == Codigos::JOGADOR_2) {
                iniciarJogadores (false, true);
                if (_jogador_2) // alocou sem problemas
                    (*_jogador_2) << in;
            }
        }
        else if (linha.compare( 0 , 15,  "LISTA_ENTIDADES") == 0) {
            EntidadeColidivel* entidade;
            while (tipo != Codigos::FIM_SAVE) {
                in >> tipo;
                switch (tipo) {
                    case Codigos::CAIXA_MADEIRA:
                        entidade = criarCaixaMadeira();
                        *entidade << in;
                        break;
                }
            }
        }
        else if (linha.compare( 0 , 15,  "LISTA_PROJETEIS") == 0) {
            Projetil* espinho;
            while (tipo != Codigos::FIM_SAVE) {
                in >> tipo;
                if (tipo == Codigos::ESPINHO) {
                    espinho = criarEspinho();
                    *espinho << in;
                }
            }
        }
        else if (linha.compare( 0, 14, "LISTA_INIMIGOS") == 0) {
            Inimigo* inimigo;
            while (tipo != Codigos::FIM_SAVE) {
                in >> tipo;
                switch (tipo) {
                    case Codigos::ESQUELETO_NU:
                        inimigo = criarEsqueletoNu();
                        *inimigo << in;
                        break;
                    case Codigos::ESQUELETO_ROUPA:
                        inimigo = criarEsqueletoRoupa();
                        *inimigo << in;
                        break;
                    case Codigos::FANTASMA:
                        inimigo = criarFantasma();
                        *inimigo << in;
                        break;
                    case Codigos::LADRAO:
                        inimigo = criarLadrao();
                        *inimigo << in;
                        break;
                }
            }
        }
        else if ("POSICAO_NAO_SPAWNADOS" == linha) {
            sf::Vector2f posicao;
            while (tipo != Codigos::FIM_SAVE && in.good()) {
                in >> tipo >> posicao;
                switch ( tipo ) {
                case Codigos::POS_FANTASMA:
                    _pos_fantasmas.push_back(posicao);
                    break; 
                case Codigos::POS_ESQUELETO_NU:
                    _pos_esqueletos_nu.push_back(posicao);
                    break; 
                case Codigos::POS_ESQUELETO_ROUPA:
                    _pos_esqueletos_roupa.push_back(posicao);
                    break; 
                case Codigos::POS_LADRAO:
                    _pos_ladroes.push_back(posicao);
                    break; 
                case Codigos::POS_CAIXA_MADEIRA:
                    _pos_caixas.push_back(posicao);
                    break; 
                case Codigos::POS_ESPINHO:
                    _pos_espinhos.push_back(posicao);
                    break; 
                }
            } 
        }
    }
    return true;
}
