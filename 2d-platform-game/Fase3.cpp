#include "Fase3.hpp"
#include "Principal.hpp"

const int Fase3::AlturaChao (Principal::Altura * 0.925);
const sf::Vector2f Fase3::SpawnJogador1 (250, AlturaChao * 0.9);
const sf::Vector2f Fase3::SpawnJogador2 (280, AlturaChao * 0.9);

Fase3::Fase3 (std::string jogador1, std::string jogador2, Janela* janela)
:   Fase (janela)
{
    // imagens de fundo o cenario
	carregarCenario();

    // chao da fase inteira
    criarChao("sprites/chao3_att.png");

    // paredes nos limetes da fase
    criarParedes();

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

    _chefao = criarDemonio (sf::Vector2f(100.0f,155.0f));
    setMusica("sounds/forward_assault.wav", 50.0f);
    _musica_de_fundo.play();
}

Fase3::Fase3 (Janela* janela)
:   Fase (janela) {
    criarChao("sprites/chao3_att.png");
    criarParedes();
	carregarCenario();
    _chefao = criarDemonio (sf::Vector2f(100.0f,200.0f));
    carregar();
    setMusica("sounds/forward_assault.wav", 50.0f);
    _musica_de_fundo.play();
}

Fase3::~Fase3() {
}

void Fase3::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw (_cenario_dinamico, states);
    Fase::draw (target,states);
}

void Fase3::atualizar() {
    Fase::atualizar();
}

void Fase3::carregarCenario() {
    std::string s_cenario_dinamico ("sprites/fundo3.png");
	_t_cenario_dinamico = carregarTextura(s_cenario_dinamico);
	_cenario_dinamico.setTexture(&_t_cenario_dinamico);
	_cenario_dinamico.setSize(sf::Vector2f(_t_cenario_dinamico.getSize()));
	_cenario_dinamico.setOrigin(0, 0);
	_cenario_dinamico.setPosition(0, 0);

	//  os limites terminam no fim do cenario
	_area_da_fase.x = 0;
	_area_da_fase.y = _t_cenario_dinamico.getSize().x;
}

Demonio* Fase3::criarDemonio(sf::Vector2f posicao) {
	Demonio* demonio = new Demonio(posicao);
	adicionar(demonio->getArtefatoAtaque());
	adicionar(static_cast <EntidadeColidivel*> (demonio));
	adicionar(static_cast <Inimigo*> (demonio));
	return demonio;
}

int Fase3::getEstado() {
    int op = Fase::getEstado();
    // retornou uma notificacao para principal 
    if (op != Codigos::CONTINUE) {
        return op;
    }
    if (_jogador_1 != NULL){
		if (_jogador_1->getPontuacao() >= 900) {
			salvarPontuacao();
			_musica_de_fundo.pause();
			return Codigos::FIM_JOGO;
		}
    }
	if (_jogador_2 != NULL) {
		if (_jogador_2->getPontuacao() >= 800) {
			salvarPontuacao();
			_musica_de_fundo.pause();
			return Codigos::FIM_JOGO;
		}
	}
    return op;
}

void Fase3::salvar() {
    std::ofstream out_fase3 ("saves/fase3.data", std::ios::out);
    if (!out_fase3) {
        std::cerr << "Fase3::salvar ";
        std::cerr << "Gravador não pode ser aberto" << std::endl;
    }
    _painel_jogador_1 >> out_fase3;
    _painel_jogador_2 >> out_fase3;
    out_fase3 << "\nJOGADORES\n";
    if (_jogador_1) {
        (*_jogador_1) >> out_fase3 << '\n';
    }
    if (_jogador_2) {
        (*_jogador_2) >> out_fase3 << '\n';
    }
    out_fase3 << Codigos::FIM_SAVE << " FIM\n";

    out_fase3 << "CHEFAO\n";
    *_chefao >> out_fase3;
    out_fase3 << Codigos::FIM_SAVE << " FIM\n";
    out_fase3 << "FIM_ARQUIVO\n";
    out_fase3.close();
}

bool Fase3::carregar() {
    std::ifstream in ("saves/fase3.data", std::ios::in);
    if (! in) {
        std::cerr << "Fase3::carregar: "
        << "nenhum arquivo foi encontrado"
        << "saves/fase3.data";
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
        else if (linha == "CHEFAO") {
            in >> tipo;
            (*_chefao) << in;
        }
    }
    return true;
}
