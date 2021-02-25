#include "Menu.hpp"
#include "Principal.hpp"

Menu::Menu()
:   _delay_para_escolha ( 0.3 )
,   _ativado (228,162,0)
,   _desativado (228,130,0)
,   _novo_jogo ("Novo Jogo", "> ", _ativado, _desativado)
,   _carregar_anterior("Carregar jogo anterior", "> ", _ativado, _desativado)
,   _mostrar_ranking ("Mostrar Ranking", "> ", _ativado, _desativado)
,   _um_jogador ("Um Jogador", "> ", _ativado, _desativado)
,   _dois_jogadores ("Dois Jogadores", "> ", _ativado, _desativado)
,   _voltar_jogo ("Voltar para o jogo", "> ", _ativado, _desativado)
,   _salvar ("Salvar", "> ", _ativado, _desativado)
,   _menu_inicial ("Menu Inicial", "> ", _ativado, _desativado)
,   _reiniciar_fase ("Reiniciar Fase", "> ", _ativado, _desativado)
,   _proxima_fase ("Proxima fase", "> ", _ativado, _desativado)
,   _sair_jogo ("Sair", "> ", _ativado, _desativado)
,   _fase_1 ("Fase 1", "> ", _ativado, _desativado)
,   _fase_2 ("Fase 2", "> ", _ativado, _desativado)
,   _fase_3 ("Fase 3", "> ", _ativado, _desativado)
,   _digite_nome_1 ("Digite o nome do jogador 1","", _ativado, _desativado)
,   _digite_nome_2 ("Digite o nome do jogador 2","", _ativado, _desativado)
,   _nome_jogador ("","", _ativado, _desativado)
,   _confirmar ("Confirmar","", _ativado, _desativado)
,   _msg_fim_jogo ("Parabens, voce completou\n      O JOGO","", _desativado, _desativado)
,   _opcao_atual ( 0 )
,   _menu_atual ( 0 )
{
    if (!_fonte_opcoes.loadFromFile("fonts/Deutsch.ttf")) {
        std::cerr << "Menu::Menu: "
        << "Erro ao carregar font "
        << "fonts/Deutsch.ttf" << std::endl;
    }

    _novo_jogo         .setFont(_fonte_opcoes);
    _carregar_anterior .setFont(_fonte_opcoes);
    _mostrar_ranking   .setFont(_fonte_opcoes);
    _um_jogador        .setFont(_fonte_opcoes);
    _dois_jogadores    .setFont(_fonte_opcoes);
    _voltar_jogo       .setFont(_fonte_opcoes);
    _salvar            .setFont(_fonte_opcoes);
    _menu_inicial      .setFont(_fonte_opcoes);
    _reiniciar_fase    .setFont(_fonte_opcoes);
    _proxima_fase      .setFont(_fonte_opcoes);
    _sair_jogo         .setFont(_fonte_opcoes);
    _fase_1            .setFont(_fonte_opcoes);
    _fase_2            .setFont(_fonte_opcoes);
    _fase_3            .setFont(_fonte_opcoes);
    _digite_nome_1     .setFont(_fonte_opcoes);
    _digite_nome_2     .setFont(_fonte_opcoes);
    _nome_jogador      .setFont(_fonte_opcoes);
    _confirmar         .setFont(_fonte_opcoes);
    _msg_fim_jogo      .setFont(_fonte_opcoes);

    _cima = sf::Keyboard::Key::Up;
    _baixo = sf::Keyboard::Key::Down;
    _enter = sf::Keyboard::Key::Return;
    _escape = sf::Keyboard::Key::Escape;

    _fundo_cor.setSize(sf::Vector2f((float) Principal::Largura, (float) Principal::Altura));
    _fundo_cor.setFillColor(sf::Color::Black);

    std::string caminho = "sprites/papiro_2.jpg";
    _t_imagem_fundo = new sf::Texture;
    if (!_t_imagem_fundo->loadFromFile(caminho)) {
        std::cerr << "Menu::Menu nao foi possivel "
        << "carregar a imagem "
        << caminho << std::endl;
    }
    _imagem_fundo.setSize(sf::Vector2f((float) Principal::Largura, (float) Principal::Altura));
    _imagem_fundo.setTexture(_t_imagem_fundo);
    
    caminho = "sprites/game-over.png";
    _t_imagem_game_over = new sf::Texture;
    if (!_t_imagem_game_over->loadFromFile(caminho)) {
        std::cerr << "Menu::Menu nao foi possivel "
        << "carregar a imagem "
        << caminho << std::endl;
    }
    _imagem_game_over.setSize(sf::Vector2f((float) Principal::Largura, (float) Principal::Altura));
    _imagem_game_over.setTexture(_t_imagem_game_over);

    if (!_bip_buffer.loadFromFile("sounds/bip.wav")) {
        std::cerr << "Menu::Menu: "
        << "Nao foi possivel carregar som "
        << "sounds/bip.wav" << std::endl;
    }
    if (!_enter_sound_buffer.loadFromFile("sounds/button_enter.wav")) {
        std::cerr << "Menu::Menu: "
        << "Nao foi possivel carregar som "
        << "sounds/button_enter.wav" << std::endl;
    }
    _bip.setBuffer(_bip_buffer);
    _enter_sound.setBuffer(_enter_sound_buffer);
}
Menu::~Menu() {
    if (_menu_atual == Codigos::RANKING) {
        destruirOpcoesRanking();
    }
    delete _t_imagem_fundo;
    delete _t_imagem_game_over;
    _t_imagem_fundo = NULL;
    _t_imagem_game_over = NULL;
}
void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_fundo_cor);
    if (_menu_atual == Codigos::GAME_OVER) {
        target.draw(_imagem_game_over);
    }
    else {
        target.draw(_imagem_fundo);
    }
    std::vector<Opcao*>::const_iterator it = _opcoes.begin();
    for (; it != _opcoes.end(); it++) {
        target.draw(*(*it), states);
    }
}
int Menu::getOpcao(int tipo_menu, std::string& nome_jogador) {
    switch (tipo_menu) {
    case Codigos::GET_NOME_JOGADOR_1:
        return getNomeJogador1(nome_jogador);
    case Codigos::GET_NOME_JOGADOR_2:
        return getNomeJogador2(nome_jogador);
    }
    return Codigos::NULO;
} 
int Menu::getOpcao(int tipo_menu) {
    if (_menu_atual == Codigos::RANKING && tipo_menu != Codigos::RANKING) {
        destruirOpcoesRanking();
    }
    if (_menu_atual == Codigos::GAME_OVER && tipo_menu != Codigos::GAME_OVER) {
        _menu_inicial.setColorAtivado(_ativado);
        _menu_inicial.setColorDesativado(_desativado);
        _sair_jogo.setColorAtivado(_ativado);
        _sair_jogo.setColorDesativado(_desativado);
        _menu_inicial.ativar();
        _sair_jogo.ativar();
    }
    switch (tipo_menu) {
    case Codigos::INICIAL:
        return inicial();
    case Codigos::JOGADORES:
        return jogadores();
    case Codigos::PAUSE:
        return pause();
    case Codigos::FASE:
        return fase();
    case Codigos::FIM_FASE:
        return fimFase();
    case Codigos::RANKING:
        return mostrarRanking();
    case Codigos::FIM_JOGO:
        return fimJogo();
    case Codigos::GAME_OVER:
        return gameOver();
    }
    return Codigos::NULO;
} 
int Menu::inicial() {
    // retornar tudo para default
    if (_menu_atual != Codigos::INICIAL) {
        _opcoes.clear();
        _opcoes.push_back(&_novo_jogo);
        _opcoes.push_back(&_carregar_anterior);
        _opcoes.push_back(&_mostrar_ranking);
        _opcoes.push_back(&_sair_jogo);
        atualizaPosicaoOpcoes();
        desativarOpcoes();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::INICIAL;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::NOVO_JOGO;
        case 1:
            return Codigos::CARREGAR_JOGO;
        case 2:
            return Codigos::MOSTRAR_RANKING;
        case 3:
            return Codigos::SAIR_JOGO;
    }
    return Codigos::NULO;
}
int Menu::mostrarRanking() {
    if (_menu_atual != Codigos::RANKING) {
        _opcoes.clear();
        pontuacoes.carregarPontuacoes();
        pontuacoes.preecherOpcoes(_opcoes);
        atualizaPosicaoOpcoes(2);
        desativarOpcoes();
        _opcoes.front()->ativar();
        for (auto& op : _opcoes) {
            op->setFont(_fonte_opcoes);
        }
        _opcao_atual = 0;
        _menu_atual = Codigos::RANKING;
    }
    if (_opcoes.empty()) {
        return Codigos::NULO;
    }
    if (_tempo_passado.getElapsedTime().asSeconds() >= _delay_para_escolha) {
        if (_opcao_atual == 0 && sf::Keyboard::isKeyPressed(_cima)) {
            _tempo_passado.restart();
            _bip.play();
            _opcoes.front()->desativar();
            pontuacoes.atualizarOpcoes(_opcoes, -1);
            _opcoes.front()->setFont(_fonte_opcoes);
            _opcoes.front()->ativar();
            atualizaPosicaoOpcoes(2);
        }
        if (_opcao_atual == (int)_opcoes.size() -1 && sf::Keyboard::isKeyPressed(_baixo)) {
            _tempo_passado.restart();
            _bip.play();
            _opcoes.back()->desativar();
            pontuacoes.atualizarOpcoes(_opcoes, 1);
            _opcoes.back()->setFont(_fonte_opcoes);
            _opcoes.back()->ativar();
            atualizaPosicaoOpcoes(2);
        }
    }
    // apenas alterna entre as opcoes
    // no raking nao precisa retornar nada
    escolha(); 
    return Codigos::NULO;
}
int Menu::jogadores() {
    // retornar tudo para default
    if (_menu_atual != Codigos::JOGADORES) {
        _opcoes.clear();
        _opcoes.push_back(&_um_jogador);
        _opcoes.push_back(&_dois_jogadores);
        desativarOpcoes();
        atualizaPosicaoOpcoes();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::JOGADORES;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::UM_JOGADOR;
        case 1:
            return Codigos::DOIS_JOGADORES;
    }
    return Codigos::NULO;
}
int Menu::pause() {
    // retornar tudo para default
    if (_menu_atual != Codigos::PAUSE) {
        _opcoes.clear();
        _opcoes.push_back(&_voltar_jogo);
        _opcoes.push_back(&_salvar);
        _opcoes.push_back(&_menu_inicial);
        desativarOpcoes();
        atualizaPosicaoOpcoes();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::PAUSE;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::VOLTAR_JOGO;
        case 1:
            return Codigos::SALVAR_JOGO;
        case 2:
            return Codigos::MENU_INICIAL;
    }
    return Codigos::NULO;
}
int Menu::fase() {
    // retornar tudo para default
    if (_menu_atual != Codigos::FASE) {
        _opcoes.clear();
        _opcoes.push_back(&_fase_1);
        _opcoes.push_back(&_fase_2);
        _opcoes.push_back(&_fase_3);
        desativarOpcoes();
        atualizaPosicaoOpcoes();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::FASE;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::FASE_1;
        case 1:
            return Codigos::FASE_2;
        case 2:
            return Codigos::FASE_3;
    }
    return Codigos::NULO;
}
int Menu::fimFase() {
    // retornar tudo para default
    if (_menu_atual != Codigos::FIM_FASE) {
        _opcoes.clear();
        _opcoes.push_back(&_reiniciar_fase);
        _opcoes.push_back(&_proxima_fase);
        _opcoes.push_back(&_menu_inicial);
        _opcoes.push_back(&_sair_jogo);
        desativarOpcoes();
        atualizaPosicaoOpcoes();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::FIM_FASE;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::REINICIAR_FASE;
        case 1:
            return Codigos::PROXIMA_FASE;
        case 2:
            return Codigos::MENU_INICIAL;
        case 3:
            return Codigos::SAIR_JOGO;
    }
    return Codigos::NULO;
}
int Menu::fimJogo() {
    // retornar tudo para default
    if (_menu_atual != Codigos::FIM_JOGO) {
        _opcoes.clear();
        _opcoes.push_back(&_msg_fim_jogo);
        _opcoes.push_back(&_menu_inicial);
        _opcoes.push_back(&_sair_jogo);
        desativarOpcoes();
        atualizaPosicaoOpcoes();
        _msg_fim_jogo.setTamanho(25);
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::FIM_JOGO;
    }
    int op = escolha();
    if (_opcao_atual == 0) {
        _opcoes.at(1)->ativar();
        _opcao_atual = 1;
    }
    switch ( op ) {
        case 0:
            return Codigos::NULO;
        case 1:
            return Codigos::MENU_INICIAL;
        case 2:
            return Codigos::SAIR_JOGO;
    }
    return Codigos::NULO;
}
int Menu::gameOver() {
    // retornar tudo para default
    if (_menu_atual != Codigos::GAME_OVER) {
        _opcoes.clear();
        _opcoes.push_back(&_menu_inicial);
        _opcoes.push_back(&_sair_jogo);
        _menu_inicial.setColorAtivado(sf::Color(255,255,255));
        _sair_jogo.setColorAtivado(sf::Color(255,255,255));
        _menu_inicial.setColorDesativado(sf::Color(255,255,255,100));
        _sair_jogo.setColorDesativado(sf::Color(255,255,255,100));
        _menu_inicial.setPosition(10,110);
        _sair_jogo.setPosition(10,140);
        _menu_inicial.ativar();
        _sair_jogo.desativar();
        _menu_inicial.setTamanho(15);
        _sair_jogo.setTamanho(15);
        _opcao_atual = 0;
        _menu_atual = Codigos::GAME_OVER;
    }
    int op = escolha();
    switch ( op ) {
        case 0:
            return Codigos::MENU_INICIAL;
        case 1:
            return Codigos::SAIR_JOGO;
    }
    return Codigos::NULO;
}
int Menu::getNomeJogador1(const std::string& str) {
    _nome_jogador.setString(str);
    // retornar tudo para default
    if (_menu_atual != Codigos::GET_NOME_JOGADOR_1) {
        _opcoes.clear();
        _opcoes.push_back(&_digite_nome_1);
        _opcoes.push_back(&_nome_jogador);
        _opcoes.push_back(&_confirmar);
        desativarOpcoes();
        _digite_nome_1.setPosition(192, 60);
        _nome_jogador .setPosition(192, 144);
        _confirmar    .setPosition(192, 250);
        _digite_nome_1.setTamanho(20);
        _nome_jogador .setTamanho(35);
        _confirmar    .setTamanho(15);
        _digite_nome_1.setOrigemNoCentro();
        _confirmar    .setOrigemNoCentro();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::GET_NOME_JOGADOR_1;
    }
    _nome_jogador.setOrigemNoCentro();
    int op = escolha();
    if (_digite_nome_1.isAtivo()) {
        _digite_nome_1.desativar();
        _nome_jogador.ativar();
        _opcao_atual = 1;
    }
    switch ( op ) {
        case 1:
            _nome_jogador.desativar();
            _confirmar.ativar();
            _opcao_atual = 2;
            break;
        case 2:
            return Codigos::GET_NOME_JOGADOR_2;
        default:
            break;
    }
    if (_nome_jogador.isAtivo()) {
        return Codigos::GET_NOME_JOGADOR_1;
    }
    return Codigos::NULO;
}
int Menu::getNomeJogador2(const std::string& str) {
    _nome_jogador.setString(str);
    // retornar tudo para default
    if (_menu_atual != Codigos::GET_NOME_JOGADOR_2) {
        _opcoes.clear();
        _opcoes.push_back(&_digite_nome_2);
        _opcoes.push_back(&_nome_jogador);
        _opcoes.push_back(&_confirmar);
        desativarOpcoes();
        _digite_nome_2.setPosition(192, 60);
        _nome_jogador .setPosition(192, 144);
        _confirmar    .setPosition(192, 250);
        _digite_nome_2.setTamanho(20);
        _nome_jogador .setTamanho(35);
        _confirmar    .setTamanho(15);
        _digite_nome_2.setOrigemNoCentro();
        _confirmar    .setOrigemNoCentro();
        _opcao_atual = 0;
        _opcoes.at(0)->ativar();
        _menu_atual = Codigos::GET_NOME_JOGADOR_2;
    }
    _nome_jogador.setOrigemNoCentro();
    int op = escolha();
    if (_digite_nome_2.isAtivo()) {
        _digite_nome_2.desativar();
        _nome_jogador.ativar();
        _opcao_atual = 1;
    }
    switch ( op ) {
        case 1:
            _nome_jogador.desativar();
            _confirmar.ativar();
            _opcao_atual = 2;
            break;
        case 2:
            return Codigos::ESCOLHER_FASE;
    }
    if (_nome_jogador.isAtivo()) {
        return Codigos::GET_NOME_JOGADOR_2;
    }
    return Codigos::NULO;
}
int Menu::escolha() {
    if (_opcoes.empty()) {
        std::cerr << "Nenhuma opcao dispinivel no menu" << std::endl;
        return Codigos::NULO;
    }
    int opcao_nova = _opcao_atual;
    if (_tempo_passado.getElapsedTime().asSeconds() >= _delay_para_escolha) {
        // caso precionou enter verificar a opcao atual
        if (sf::Keyboard::isKeyPressed(_enter)) {
            _tempo_passado.restart();
            _enter_sound.play();
            return _opcao_atual;
        }
        // verificar as outras teclas ( cima / baixo )
        if (sf::Keyboard::isKeyPressed(_cima)) {
            _tempo_passado.restart();
            _opcao_atual == 0 ? opcao_nova += _opcoes.size() - 1 : opcao_nova--;
            _bip.play();
        }
        else if (sf::Keyboard::isKeyPressed(_baixo)) {
            _tempo_passado.restart();
            opcao_nova = (_opcao_atual + 1) % _opcoes.size();
            _bip.play();
        }
    }
    // destacar a opcao selecionada
    if (opcao_nova != _opcao_atual) {
        _opcoes[_opcao_atual]->desativar();
        _opcoes[opcao_nova]->ativar();
        _opcao_atual = opcao_nova;
    }
    return Codigos::NULO;
}

void Menu::atualizaPosicaoOpcoes(int escala) {
    // calcular o tamanho e posicao usando usando uma
    // o tamanho da janela
    int borda = Principal::Altura / 8;
    int altura = Principal::Altura - 2.5 * borda;
    int tam = altura / (_opcoes.size() + 1);
    int left = Principal::Largura / 8;
    int borda_esquerda = Principal::Largura / 6;
    int tam_texto = (Principal::Largura - 2 * borda_esquerda) / 25;
    for (std::size_t i = 0; i < _opcoes.size(); i++) {
        _opcoes[i]->setPosition(left , borda + tam * (i+1));
        _opcoes[i]->setTamanho(tam_texto * escala);
    }
}

void Menu::desativarOpcoes() {
    for (std::size_t i = 0; i < _opcoes.size(); i++) {
        if (_opcoes[i]->isAtivo()) {
            _opcoes[i]->desativar();
        }
    }
}
void Menu::destruirOpcoesRanking() {
    std::vector<Opcao*>::iterator it = _opcoes.begin();
    while (it != _opcoes.end()) {
        delete (*it);
        it++;
    }
    _opcoes.clear();
}
