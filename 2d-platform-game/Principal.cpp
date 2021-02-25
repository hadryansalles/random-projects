#include "Principal.hpp"
#include <stdexcept>

const int Principal::Largura ( 384 );
const int Principal::Altura ( 288 );
const std::string Principal::NomeJogo ("Jogo");
const sf::Vector2u Principal::Resolucao (1360, 768);
const int Principal::FpsMaximo (60);

Principal::Principal()
:   _janela (Resolucao.x, Resolucao.y, NomeJogo)
,   _menu()
,	_fase_atual( NULL )
,	_rodando ( false )
,   _id_fase_atual ( Codigos::NULO )
,   _menu_atual ( Codigos::INICIAL )
,   _jogador_1 (false)
,   _jogador_2 (false) 
,   _nome_jogador_1 ("")
,   _nome_jogador_2 ("")
{
   _nome_jogador_2.clear(); 
   _nome_jogador_1.clear(); 
}

Principal::~Principal() {
    destruirFaseAtual();
	_rodando = false;
}

void Principal::executar() {
	//enquanto nao pressionar no X da janela
	while (_janela.estaAberta())
	{
        _janela.clear();
        // Enquando existir uma fase em andamento e
        // a janela nao foi fechada
		while (this->estaRodando() && _janela.estaAberta()) {
            // limpa e atualiza a janela
            _janela.clear();
            _janela.atualizar();
            _janela.desenhar (*_fase_atual);
            _janela.atualizarCamera(_fase_atual->getArea());

            // atualiza a fase atual
			_fase_atual->atualizar();
            
            // verifica o estado da fase
            int op = _fase_atual->getEstado();
            switch ( op ) {
            case Codigos::CONTINUE:
                break;
            case Codigos::GAME_OVER:
                _rodando = false;
                _menu_atual = Codigos::GAME_OVER;
                break;
            case Codigos::FIM_FASE:
                _rodando = false;
                _menu_atual = (_id_fase_atual == Codigos::FASE_3 ? Codigos::FIM_JOGO : Codigos::FIM_FASE);
                break;
            case Codigos::PAUSE:
                _rodando = false;
                _menu_atual = Codigos::PAUSE;
                break;
            case Codigos::FIM_JOGO:
                _rodando = false;
                _menu_atual = Codigos::FIM_JOGO;
                break;
            }
            // troca o buffer da janela imprimindo os
            // objetos desenhados
            _janela.renderiza();
        }
		menu();
        _janela.renderiza();
	}
}
void Principal::destruirFaseAtual() {
	if (_fase_atual != NULL) {
        delete _fase_atual;
        _fase_atual = NULL;
        _id_fase_atual = Codigos::NULO;
	}
}
void Principal::menu() {
    // retira o foco da camera do personagem e
    // volta para o centro da janela
    _janela.resetarCamera();
    if (_menu_atual != Codigos::GET_NOME_JOGADOR_1 && _menu_atual != Codigos::GET_NOME_JOGADOR_2) {
        _janela.atualizar();
    }
    // obtem a o opcao selecionada pelo usuario no menu
	int opcao = _menu.getOpcao(_menu_atual);
    switch ( _menu_atual ) {
        case Codigos::JOGADORES:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                _menu_atual = Codigos::INICIAL;
            }
            break;
        case Codigos::FASE:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                _menu_atual = Codigos::GET_NOME_JOGADOR_2;
            }
            break;
        case Codigos::GET_NOME_JOGADOR_1:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                _menu_atual = Codigos::JOGADORES;
            }
            opcao = _menu.getOpcao(_menu_atual, _nome_jogador_1);
            _janela.getString(_nome_jogador_1, (opcao == Codigos::GET_NOME_JOGADOR_1));
            break;
        case Codigos::GET_NOME_JOGADOR_2:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                _menu_atual = Codigos::GET_NOME_JOGADOR_1;
            }
            opcao = _menu.getOpcao(_menu_atual, _nome_jogador_2);
            _janela.getString(_nome_jogador_2, (opcao == Codigos::GET_NOME_JOGADOR_2));
            break;
        case Codigos::RANKING:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                _menu_atual = Codigos::INICIAL;
            }
            break;
    }

    _janela.desenhar (_menu);

	switch (opcao) {
    case Codigos::NULO:
        return;
    case Codigos::NOVO_JOGO:
        _menu_atual = Codigos::JOGADORES;
        break;
	case Codigos::UM_JOGADOR:
        _jogador_1 = true;
        _jogador_2 = false;
        _menu_atual = Codigos::GET_NOME_JOGADOR_1;
		break;
	case Codigos::DOIS_JOGADORES:
        _jogador_1 = true;
        _jogador_2 = true;
        _menu_atual = Codigos::GET_NOME_JOGADOR_1;
		break;
	case Codigos::CARREGAR_JOGO:
        carregar();
		break;
	case Codigos::SALVAR_JOGO:
		salvar();
		break;
    case Codigos::VOLTAR_JOGO:
        _fase_atual->continuarJogo();
        _rodando = true;
        break;
    case Codigos::MENU_INICIAL:
        _menu_atual = Codigos::INICIAL;
        break;
	case Codigos::SAIR_JOGO:
		_rodando = false;
        _janela.close();
		break;
    case Codigos::MOSTRAR_RANKING:
        _menu_atual = Codigos::RANKING;
		break;
    case Codigos::REINICIAR_FASE:
		iniciarJogo(_id_fase_atual, _jogador_1, _jogador_2);
		break;
    case Codigos::PROXIMA_FASE:
        if (_id_fase_atual != Codigos::FASE_3) {
            std::pair<bool,bool> existe;
            existe = _fase_atual->getExistenciaJogadores();
            iniciarJogo(_id_fase_atual + 1, existe.first, existe.second);
        }
		break;
    case Codigos::GAME_OVER:
        _menu_atual = Codigos::INICIAL;
        break;
    case Codigos::ESCOLHER_FASE:
        _menu_atual = Codigos::FASE;
        break;
    case Codigos::FASE_1:
		iniciarJogo(Codigos::FASE_1, _jogador_1, _jogador_2);
		break;
    case Codigos::FASE_2:
		iniciarJogo(Codigos::FASE_2, _jogador_1, _jogador_2);
		break;
    case Codigos::FASE_3:
		iniciarJogo(Codigos::FASE_3, _jogador_1, _jogador_2);
		break;
    case Codigos::GET_NOME_JOGADOR_2:
        if (_jogador_2) {
            _menu_atual = Codigos::GET_NOME_JOGADOR_2;
        }
        else {
            _menu_atual = Codigos::FASE;
        }
	}
}

bool Principal::estaRodando() {
	return _rodando;
}

void Principal::iniciarJogo(int id_fase, bool jogador_1, bool jogador_2) {
    Fase* fase_nova;
    bool nova = true;
    // nao foi passado jogador, entao ira carragar uma fase ja iniciada
    if (!jogador_1 && !jogador_2) {
        nova = false;
    }
    if (_jogador_1 && _nome_jogador_1.empty()) {
        _nome_jogador_1 = "Jogador 1";
    }
    if (_jogador_2 && _nome_jogador_2.empty()) {
        _nome_jogador_2 = "Jogador 2";
    }
	if (!_jogador_2) {
		_nome_jogador_2.clear();
	}
	//inicia outros personagens e entidades
	switch (id_fase) {
    case Codigos::FASE_1:
        if (nova) {
            fase_nova = static_cast <Fase*> (new Fase1(_nome_jogador_1, _nome_jogador_2, &_janela));
        }
        else {
            fase_nova = static_cast <Fase*> (new Fase1(&_janela));
        }
        destruirFaseAtual();
        _id_fase_atual = id_fase;
        _fase_atual = fase_nova;
        break;
    case Codigos::FASE_2:
        if (nova) {
            fase_nova = static_cast <Fase*> (new Fase2(_nome_jogador_1, _nome_jogador_2, &_janela));
        }
        else {
            fase_nova = static_cast <Fase*> (new Fase2(&_janela));
        }
        destruirFaseAtual();
        _id_fase_atual = id_fase;
        _fase_atual = fase_nova;
        break;
    case Codigos::FASE_3:
        if (nova) {
            fase_nova = static_cast <Fase*> (new Fase3(_nome_jogador_1, _nome_jogador_2, &_janela));
        }
        else {
            fase_nova = static_cast <Fase*> (new Fase3(&_janela));
        }
        destruirFaseAtual();
        _id_fase_atual = id_fase;
        _fase_atual = fase_nova;
        break;
     default:
            std::cerr << "Principal::iniciarJogo "
            << "Esta fase nao existe" << std::endl;
	}
    if (_fase_atual != NULL) {
        // nao ocorreu erros na alocacao da fase
        _rodando = true;
    }
    else {
        std::cerr << "Principal::iniciarJogo: ";
        std::cerr << "Nao foi possivel iniciar a fase" << std::endl;
    }
}

void Principal::salvar() {
    if (_fase_atual) {
        _fase_atual->salvar();
    }
    std::ofstream out ("saves/fase.data", std::ios::out);
    out << _id_fase_atual;
    out.close();
}

void Principal::carregar() {
    std::ifstream in ("saves/fase.data", std::ios::in);
    in >> _id_fase_atual;
    iniciarJogo(_id_fase_atual);
}
