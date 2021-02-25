#include "Fase1.hpp"
#include "Principal.hpp"

const int Fase1::AlturaChao (Principal::Altura * 0.925);
const sf::Vector2f Fase1::SpawnJogador1 (100, AlturaChao * 0.9);
const sf::Vector2f Fase1::SpawnJogador2 ( 70, AlturaChao * 0.9);

Fase1::Fase1 (std::string jogador1, std::string jogador2, Janela* janela) 
:   Fase (janela , sf::IntRect (2892, 50, 40, 60)) {

    // imagens de fundo o cenario
	carregarCenario();

    spawnarCaixasMadeira();

    // chao da fase inteira
    criarChao();

    // marquises das casas
    criarMarquises();

    // paredes nos limetes da fase
    criarParedes();

    // escolhe as posicoes dos esqueletos na fase    
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
Fase1::Fase1 (Janela* janela)
:   Fase (janela , sf::IntRect (2892, 50, 40, 60)) {
	carregarCenario();
    criarChao();
    criarMarquises();
    criarParedes();
    carregar();
    setMusica("sounds/forward_assault.wav", 50.0f);
    _janela->atualizarCamera();
    _musica_de_fundo.play();
}
Fase1::~Fase1() {
}
void Fase1::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw (_cenario_estatico, states);
	target.draw (_cenario_dinamico, states);
    Fase::draw (target,states);
}
void Fase1::atualizar() {
	_cenario_estatico.setPosition(_janela->getCameraPos());
    spawnarInimigos();
    Fase::atualizar();
}
void Fase1::spawnarInimigos() {
    spawnarEsqueletosNu();
    spawnarEsqueletosRoupa();
    spawnarFantasmas();
}
void Fase1::spawnarFantasmas() {
    std::vector<sf::Vector2f>::iterator pos = _pos_fantasmas.begin();
    while (pos != _pos_fantasmas.end()) {
       if (temJogadorProximo(*pos)) {
            criarFantasma (*pos);
            pos = _pos_fantasmas.erase(pos);
        }
        else pos++;
    }
}
void Fase1::spawnarEsqueletosRoupa() {
    std::vector<sf::Vector2f>::iterator pos = _pos_esqueletos_roupa.begin();
    while (pos != _pos_esqueletos_roupa.end()) {
       if (temJogadorProximo(*pos)) {
            criarEsqueletoRoupa (*pos);
            pos = _pos_esqueletos_roupa.erase(pos);
        }
        else pos++;
    }
}
void Fase1::spawnarEsqueletosNu() {
    std::vector<sf::Vector2f>::iterator pos = _pos_esqueletos_nu.begin();
    while (pos != _pos_esqueletos_nu.end()) {
       if (temJogadorProximo(*pos)) {
            criarEsqueletoNu (*pos);
            pos = _pos_esqueletos_nu.erase(pos);
        }
        else pos++;
    }
}
void Fase1::spawnarCaixasMadeira() {
    criarCaixaMadeira(3000, 175);
    criarCaixaMadeira(3000, 140);
    criarCaixaMadeira(3020, 105);
    criarCaixaMadeira(2961, 105);
    criarCaixaMadeira(20  , 105);
    criarCaixaMadeira(2800, 105);
    criarCaixaMadeira(1500, 105);
    criarCaixaMadeira(1057, 105);
}
void Fase1::sorteiaPosicaoEntidades() {
    srand(time(NULL));
    int i = 0;
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_esqueletos_nu.push_back (sf::Vector2f(200 + i, AlturaChao * 0.9));
    } while (( i % 51 || _pos_esqueletos_nu.size() < 5) && _pos_esqueletos_nu.size() < 10);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_esqueletos_roupa.push_back (sf::Vector2f(200 + i , AlturaChao * 0.9));
    } while (( i % 51 || _pos_esqueletos_roupa.size() < 5) && _pos_esqueletos_roupa.size() < 10);
    do {
        i = rand() % (_fim_fase.left - 300);
        _pos_fantasmas.push_back (sf::Vector2f( 200 + i, Principal::Altura * 0.5));
    } while (( i % 51 || _pos_fantasmas.size() < 5) && _pos_fantasmas.size() < 10);
}
void Fase1::criarMarquises() {
    EntidadeColidivel* marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(2912,110)); //medido
    marquise->setArea (sf::Vector2f(48.0f, 20.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(1643.0f,170.0f)); //medido
    marquise->setArea (sf::Vector2f(117.0f, 10.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(151.0f,172.0f)); //medido
    marquise->setArea (sf::Vector2f(116.0f, 10.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(338.0f,172.0f)); //medido
    marquise->setArea (sf::Vector2f(160.0f, 15.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(784.0f,172.0f)); //medido
    marquise->setArea (sf::Vector2f(160.0f, 15.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(973.0f,172.0f)); //medido
    marquise->setArea (sf::Vector2f(116.0f, 10.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );

    marquise = new EntidadeColidivel;
    marquise->setPosicao (sf::Vector2f(1833.0f,172.0f)); //medido
    marquise->setArea (sf::Vector2f(160.0f, 15.0f));
    marquise->setEstatico(true);
    adicionar ( marquise );
}
void Fase1::carregarCenario() {
    std::string s_cenario_estatico("sprites/cenario1.png");
    std::string s_cenario_dinamico("sprites/cenario-dinamico-1.png");
	_t_cenario_estatico = carregarTextura(s_cenario_estatico);
	_cenario_estatico.setSize(sf::Vector2f(Principal::Largura, Principal::Altura));
	_cenario_estatico.setTexture(&_t_cenario_estatico);
	_cenario_estatico.setOrigin(_t_cenario_estatico.getSize().x / 2, _t_cenario_estatico.getSize().y / 2);
	_cenario_estatico.setPosition(0, 0);
	_t_cenario_dinamico = carregarTextura(s_cenario_dinamico);
	_cenario_dinamico.setTexture(&_t_cenario_dinamico);
	_cenario_dinamico.setSize(sf::Vector2f(_t_cenario_dinamico.getSize()));
	_cenario_dinamico.setOrigin(0, 0);
	_cenario_dinamico.setPosition(0, 0);

	//  os limites terminam no fim do cenario em primiro plano
	_area_da_fase.x = 0;
	_area_da_fase.y = _t_cenario_dinamico.getSize().x;

}
void Fase1::criarChao() {
//    primeiro chao em x = 0 para pegar a largura de cada bloco
    // O chao sera criado pouco antes da borda da janela 
    EntidadeColidivel* chao = criarBlocoChao ( 0.0f , Principal::Altura * 0.925f);
    float largura_chao = chao->getSize().x;
    for (int i = 1; i <= 1 + _area_da_fase.y / largura_chao; i++) {
        if (i != 38 && i != 71) { // 2 espaco
            switch (i) {
                case 12:
                    criarBlocoChao ((i - 0.1) * largura_chao, Principal::Altura * 0.925f, 0.5);
                    break;
                case 13:
                    criarBlocoChao ((i + 0.5)* largura_chao,  Principal::Altura * 0.925f, 0.6);
                    break;
                default:
                    criarBlocoChao (i * largura_chao, Principal::Altura * 0.925f);
            }
        }
        else {
            i++;
        }
    }
}
Fantasma* Fase1::criarFantasma(sf::Vector2f posicao) {
    Fantasma* fantasma = new Fantasma (posicao);
    adicionar (fantasma->getArtefatoAtaque());
    adicionar (static_cast <EntidadeColidivel*> (fantasma));
    adicionar (static_cast <Inimigo*> (fantasma));
    return fantasma;
}
EntidadeColidivel* Fase1::criarBlocoChao(float pos_x, float pos_y, float escala) {
    EntidadeColidivel* chao = new EntidadeColidivel(Codigos::CHAO);
    if ( _bloco_chao == NULL ) {
        _bloco_chao = chao->carregarTextura ("sprites/chao.png");
    }
    chao
        ->setTextura ( _bloco_chao )
        ->setIdTipoObjeto ( Codigos::CHAO )
        ->setCoeficiente(1.0f)
        ->setEstatico(true)
        ->setOrigem((float)_bloco_chao->getSize().x / 2, (float)_bloco_chao->getSize().y / 2)
        ->setArea(sf::Vector2f((float)_bloco_chao->getSize().x * escala, (float)_bloco_chao->getSize().y))
        ->setSize(sf::Vector2f((float)_bloco_chao->getSize().x * escala, (float)_bloco_chao->getSize().y))
        ->setPosicao(pos_x - (chao->getArea().x * (1.0 - escala)), pos_y);
    adicionar (static_cast<EntidadeColidivel*> (chao));
    return ( chao );
}
void Fase1::salvar() {
    std::ofstream out_fase1 ("saves/fase1.data", std::ios::out);
    if (!out_fase1) {
        std::cerr << "Fase1::salvar ";
        std::cerr << "Gravador não pode ser aberto" << std::endl;
    }
    _painel_jogador_1 >> out_fase1;
    _painel_jogador_2 >> out_fase1;
    out_fase1 << "\nJOGADORES\n";
    if (_jogador_1) {
        (*_jogador_1) >> out_fase1 << '\n';
    }
    if (_jogador_2) {
        (*_jogador_2) >> out_fase1 << '\n';
    }
    out_fase1 << Codigos::FIM_SAVE << " FIM\n";
    
    out_fase1 << "LISTA_ENTIDADES\n";
    _entidades >> out_fase1;
    out_fase1 << Codigos::FIM_SAVE << " FIM\n";

    out_fase1 << "LISTA_INIMIGOS\n";
    _inimigos >> out_fase1;
    out_fase1 << Codigos::FIM_SAVE << " FIM\n";

    out_fase1 << "POSICAO_NAO_SPAWNADOS\n";
    std::vector<sf::Vector2f>::iterator it = _pos_esqueletos_roupa.begin();
    for (; it != _pos_esqueletos_roupa.end(); it++) {
        out_fase1 <<Codigos::POS_ESQUELETO_ROUPA << ' ' << (*it) << '\n';
    }
    it = _pos_esqueletos_nu.begin();
    for (; it != _pos_esqueletos_nu.end(); it++) {
        out_fase1 << Codigos::POS_ESQUELETO_NU << ' ' << (*it) << '\n';
    }
    it = _pos_fantasmas.begin();
    for (; it != _pos_fantasmas.end(); it++) {
        out_fase1 << Codigos::POS_FANTASMA << ' ' << (*it) << '\n';
    }
    out_fase1 << Codigos::FIM_SAVE << " FIM\n";
    out_fase1 << "FIM_ARQUIVO\n";
    out_fase1.close();
}
bool Fase1::carregar() {
    std::ifstream in ("saves/fase1.data", std::ios::in);
    if (! in) {
        std::cerr << "Fase1::carregar: "
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
                }
            } 
        }
    }
    return true;
}
