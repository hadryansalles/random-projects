#include "Ranking.hpp"

Ranking::Ranking()
:   pontuacoes () 
,   _opcoes_disponiveis () {
}

Ranking::~Ranking() {
}

void Ranking::carregarPontuacoes() {
    pontuacoes.clear();
    std::string caminho = "saves/Ranking.data";
    std::ifstream in (caminho, std::ios::in);
    if (!in) {
        std::cerr << "Nao foi possivel abrir o arquivo: " 
        << caminho << std::endl;
    }
    std::string pontos, nome;
    getline(in, pontos, '|');
    getline(in, nome, '|');
    while (in.good()) {
        try {
            pontuacoes.insert(make_pair(std::stof(pontos), nome));
        } catch (std::exception e) {
            std::cerr << e.what() << std::endl;
            std::cerr << "Erro ao obter pontuacao do jogador " << nome << std::endl;
        }
        getline(in, nome, '\n');
        getline(in, pontos, '|');
        getline(in, nome, '|');
    }
} 

void Ranking::preecherOpcoes(std::vector<Opcao*>& opcoes) {
    std::multimap<float,std::string>::iterator it;
    int i = 0;
    std::string str;
    _opcoes_disponiveis.first = pontuacoes.begin();
    for (it = pontuacoes.begin();i < 6 && it != pontuacoes.end(); it++ , i++) {
        str = std::to_string((*it).first);
        str.erase (str.end() - 4, str.end());
        str += " " + (*it).second;
        opcoes.push_back(new Opcao (str, "  ", sf::Color(228,162,0), sf::Color(228,130,0)));
        _opcoes_disponiveis.second = it;
    }
    if (opcoes.empty()) {
        str = "Nao existe dados no ranking";
        opcoes.push_back(new Opcao (str, "  ", sf::Color(228,162,0), sf::Color(228,130,0)));
    }
}

void Ranking::atualizarOpcoes(std::vector<Opcao*>& opcoes, int direcao) {
    if (pontuacoes.size() <= 6) {
        return;
    }
    std::multimap<float,std::string>::iterator it;
    std::string str;
    it = _opcoes_disponiveis.second;
    it++;
    if (direcao > 0 && it != pontuacoes.end()) {
        delete opcoes.front();
        opcoes.erase(opcoes.begin());
        _opcoes_disponiveis.first++;
        _opcoes_disponiveis.second++;
        str = std::to_string((*(_opcoes_disponiveis.second)).first);
        str.erase (str.end() - 4, str.end());
        str += " " + (*(_opcoes_disponiveis.second)).second;
        opcoes.push_back(new Opcao (str, "  ", sf::Color(228,162,0), sf::Color(228,130,0)));
    }
    it = _opcoes_disponiveis.first;
    if (direcao < 0 && it != pontuacoes.begin()) {
        delete opcoes.back();
        opcoes.pop_back();
        _opcoes_disponiveis.first--;
        _opcoes_disponiveis.second--;
        str = std::to_string((*(_opcoes_disponiveis.first)).first);
        str.erase (str.end() - 4, str.end());
        str += " " + (*(_opcoes_disponiveis.first)).second;
        opcoes.insert(opcoes.begin(), (new Opcao (str, "  ", sf::Color(228,162,0), sf::Color(228,130,0))));
    }
}
