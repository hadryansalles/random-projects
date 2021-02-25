#include "Std.hpp"
#include "Entidade.hpp"
#include "Opcao.hpp"

class Ranking {
private:
    std::multimap<float, std::string, std::greater<float> > pontuacoes;
    std::pair<std::multimap<float,std::string>::iterator,
        std::multimap<float,std::string>::iterator> _opcoes_disponiveis;
public:
    Ranking();
    ~Ranking();
    void carregarPontuacoes();
    void preecherOpcoes(std::vector<Opcao*>& opcoes);
    void atualizarOpcoes(std::vector<Opcao*>& opcoes, int direcao);
};
