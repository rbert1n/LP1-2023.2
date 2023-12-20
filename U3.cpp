#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

class CampoMinado {
    
private:
  int Comprimento;
  int Largura;
  double Area;
  int Bombas;
  double Densidade;
  string NomeJogador;
  int pontuacao;
  vector<vector<char>> matrizCampo;
  vector<vector<bool>> camposRevelados;

public:
    void iniciarJogo() {
        pontuacao = 0;
        definirNome();
        definirTamanho();
        escolherDificuldade();
        criarMatrizCampo();
        distribuiBombas();
        preencherValores();
        criarCamposRevelados();
        time_t inicio = time(nullptr);
        jogar();
        time_t fim = time(nullptr);
        int tempoDecorrido = difftime(fim, inicio);
        pontuacao += calcularPontuacao(tempoDecorrido, Densidade);
        cout << "Pontuação final: " << pontuacao << endl;
        imprimeCampos();
    }

    void definirNome() {
        cout << "Ola, vamos jogar campo minado!\nPrimeiro me informe seu nome:\n";
        cin >> NomeJogador;
        //NomeJogador = nome;
    }
    
    // Função onde o jogador escolhe o tamanho do campo
    void definirTamanho() {
        cout << "Agora escolha o tamanho do campo você quer jogar\n";
        cout << "Primeiro me informe o Comprimento\n";
        cin >> Comprimento;
        while (Comprimento <= 5) {
            cout << "Desculpa o Comprimento não pode ser menor ou igual a 5, me diz um Comprimento diferente.\n";
            cin >> Comprimento;
        }
        cout << "Show, agora me informe a Largura\n";
        cin >> Largura;
        while (Largura <= 5) {
            cout << "Desculpa a Largura não pode ser menor ou igual a 5, me diz uma Largura diferente.\n";
            cin >> Largura;
        }
        while (Largura > Comprimento) {
            cout << "Desculpa a Largura não pode ser maior que o Comprimento, me diz uma Largura diferente.\n";
            cin >> Largura;
        }
        Area = Comprimento * Largura;
    }
    
    // Função onde o jogador escolhe a dificuldade que é baseada na densidade de bombas
    void escolherDificuldade() {
        int dificuldade;
        cout << "Ótimo! Antes de começar, escolha sua dificuldade\n";
        cout << "1 - Fácil\n2 - Médio\n3 - Difícil\n4 - Personalizada\n5 - Explicação\n";
        //Fácil = 10%
        //Médio = 20%
        //Difícil = 30%
        cin >> dificuldade;
        while (dificuldade < 0 || dificuldade > 5) {
            cout << "Valor inválido\n";
            cout << "1 - Fácil\n2 - Médio\n3 - Difícil\n4 - Personalizada\n5 - Explicação\n";
            cin >> dificuldade;
        }
        if (dificuldade == 1) {
            cout << "Dificuldade definida\n";
            defineDensidade(0.1);
        } else if (dificuldade == 2) {
            cout << "Dificuldade definida\n";
            defineDensidade(0.2);
        } else if (dificuldade == 3) {
            cout << "Dificuldade definida\n";
            defineDensidade(0.3);
        } else if (dificuldade == 4) {
            float porcentagemPersonalizada;
            cout << "Dificuldade definida\n";
            cin >> porcentagemPersonalizada;
            defineDensidade(porcentagemPersonalizada);
        } else if (dificuldade == 5) {
            cout << "A dificuldade define a densidade de bombas dentro da área do campo minado, sendo elas:\n";
            cout << "Fácil: 10% das células vão ser bombas.\n";
            cout << "Médio: 20% das células vão ser bombas.\n";
            cout << "Difícil: 30% das células vão ser bombas.\n";
            cout << "Personalizada: Você que define a densidade de bombas (Só não pode ser maior que 50% kkk)\n";
            cin >> dificuldade;
            if (dificuldade == 1) {
                defineDensidade(0.1);
            } else if (dificuldade == 2) {
                defineDensidade(0.2);
            } else if (dificuldade == 3) {
                defineDensidade(0.3);
            } else {
                float porcentagemPersonalizada;
                cout << "Dificuldade definida\n";
                cin >> porcentagemPersonalizada;
                defineDensidade(porcentagemPersonalizada);
            }
        }
    }
    
    void defineDensidade(float fator) {
        Bombas = Area * fator;
        Densidade = (Bombas / Area) * 100;
    }
    
    void imprimeCampos() const{
        cout << "Comprimento: " << Comprimento << "\n";
        cout << "Largura: " << Largura << "\n";
        cout << "Area: " << Area << "\n";
        cout << "Bombas: " << Bombas << "\n";
        cout << "Densidade: " << Densidade << "%\n";
        cout << "Nome do Jogador: " << NomeJogador << "\n";
    }

    // Aloca memória para o campo minado em si
    void criarMatrizCampo() {
        matrizCampo.resize(Comprimento, vector<char>(Largura, '0'));
    }
    
    // Aloca memória para definir se a célula do campo está visível ou não
    void criarCamposRevelados() {
        camposRevelados.resize(Comprimento, vector<bool>(Largura, false));
    }
    
    // Distribui as bombas na matriz do campo aleatoriamente
    void distribuiBombas() {
        srand(static_cast<unsigned int>(time(0)));

        int bombasRestantes = Bombas;

        while (bombasRestantes > 0) {
            int linha = rand() % Comprimento;
            int coluna = rand() % Largura;
            if (matrizCampo[linha][coluna] != 'B') {
                matrizCampo[linha][coluna] = 'B';
                bombasRestantes--;
            }
        }
    }
    
    // Preenche os campos ao redor das bombas
    void preencherValores() {
        for (int i = 0; i < Comprimento; ++i) {
            for (int j = 0; j < Largura; ++j) {
                if (matrizCampo[i][j] != 'B') {
                    int bombasAdjacentes = contarBombasAdjacentes(i, j);
                    matrizCampo[i][j] = (bombasAdjacentes > 0) ? ('0' + bombasAdjacentes) : '0';
                }
            }
        }
    }

    // Auxiliar da função acima, faz a contagem a ser preenchida
    int contarBombasAdjacentes(int linha, int coluna) const {
        int bombasAdjacentes = 0;

        int posicoes[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},            {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        for (int i = 0; i < 8; ++i) {
            int novaLinha = linha + posicoes[i][0];
            int novaColuna = coluna + posicoes[i][1];
            
            if (novaLinha >= 0 && novaLinha < Comprimento && novaColuna >= 0 && novaColuna < Largura) {
                if (matrizCampo[novaLinha][novaColuna] == 'B') {
                    bombasAdjacentes++;
                }
            }
        }

        return bombasAdjacentes;
    }

    // Imprime o campo em si
    void imprimirMatriz() const {
        cout << "Matriz do Campo Minado:\n";
        cout << "  ";
        for (int j = 0; j < Largura; ++j) {
            char letra = 'A' + j;
            cout << "   " << letra;
        }
        cout << "\n";
        for (int i = 0; i < Comprimento; ++i) {
            cout << "   ";
            for (int j = 0; j < Largura; ++j) {
                cout << "----";
            }
            cout << "-\n";
            if (i > 8) {
                cout << i + 1 << " ";
            } else {
                cout << " " << i + 1 << " ";
            }
            for (int j = 0; j < Largura; ++j) {
                cout << "| " << matrizCampo[i][j] << " ";
            }
            cout << "|\n";
        }
        cout << "   ";
        for (int j = 0; j < Largura; ++j) {
            cout << "----";
        }
        cout << "-\n";
    }
    
    // Imprime o campo para o jogador, verificando se a célula está oculta ou não
    void imprimeMatrizJogador() const {
        cout << "Matriz do Campo Minado:\n";
        cout << "  ";
        for (int j = 0; j < Largura; ++j) {
            char letra = 'A' + j;
            cout << "   " << letra;
        }
        cout << "\n";
        for (int i = 0; i < Comprimento; ++i) {
            cout << "   ";
            for (int j = 0; j < Largura; ++j) {
                cout << "----";
            }
            cout << "-\n";
            if (i > 8) {
                cout << i + 1 << " ";
            } else {
                cout << " " << i + 1 << " ";
            }
            for (int j = 0; j < Largura; ++j) {
                if (camposRevelados[i][j]) {
                    cout << "| " << matrizCampo[i][j] << " ";
                } else {
                    cout << "|   ";
                }
            }
            cout << "|\n";
        }
        cout << "   ";
        for (int j = 0; j < Largura; ++j) {
            cout << "----";
        }
        cout << "-\n";
    }
    
    // Verifica se o campo já foi revelado e se contêm bomba
    bool revelarCampo(int linha, int coluna) {
        if (camposRevelados[linha][coluna]) {
            cout << "Campo já revelado. Tente novamente.\n";
            return false;
        }
        
        camposRevelados[linha][coluna] = true;

        if (matrizCampo[linha][coluna] == 'B') {
            return true;
        }

        if (matrizCampo[linha][coluna] == '0') {
            revelarCamposVizinhos(linha, coluna);
        }

        return false;
    }

    // Revela as posições vizinhas
    void revelarCamposVizinhos(int linha, int coluna) {
        int posicoes[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},            {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        for (int i = 0; i < 8; ++i) {
            int novaLinha = linha + posicoes[i][0];
            int novaColuna = coluna + posicoes[i][1];

            if (novaLinha >= 0 && novaLinha < Comprimento && novaColuna >= 0 && novaColuna < Largura) {
                if (!camposRevelados[novaLinha][novaColuna]) {
                    camposRevelados[novaLinha][novaColuna] = true;

                    if (matrizCampo[novaLinha][novaColuna] == '0') {
                        revelarCamposVizinhos(novaLinha, novaColuna);
                    }
                }
            }
        }
    }
    
    // Verifica se todos os campos não contendo bombas foram revelados
    bool verificarVitoria() const {
        for (int i = 0; i < Comprimento; ++i) {
            for (int j = 0; j < Largura; ++j) {
                if (matrizCampo[i][j] != 'B' && !camposRevelados[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Função que pergunta a posição a ser jogada e utiliza as demais funções para rodar o jogo
    void jogar() {
        while (true) {
            system("clear");
            // imprimirMatriz();
            imprimeMatrizJogador();

            cout << "Digite a posição que deseja revelar (ex: D5): ";
            string posicao;
            cin >> posicao;

            int coluna = posicao[0] - 'A';
            int linha = stoi(posicao.substr(1)) - 1;

            if (linha >= 0 && linha < Comprimento && coluna >= 0 && coluna < Largura) {
                bool acertouBomba = revelarCampo(linha, coluna);

                if (acertouBomba) {
                    imprimirMatriz();
                    cout << "Você acertou uma bomba! Fim de jogo.\n";
                    break;
                }

                if (verificarVitoria()) {
                    imprimirMatriz();
                    cout << "Parabéns! Você venceu!\n";
                    break;
                }
            } else {
                cout << "Posição inválida. Tente novamente.\n";
            }
        }
    }
    
    // Calcula a pontuação com base no número de campos revelados e no tempo decorrido
    int calcularPontuacao(int tempoDecorrido, double Densidade) const {
        int pontuacaoBase = static_cast<int>(1000 * Densidade); // Pontuação por campo revelado e por densidade
        int pontuacaoTempo = 10; // Pontuação por segundo
        
        return (camposRevelados.size() * camposRevelados[0].size() * pontuacaoBase) - (tempoDecorrido * pontuacaoTempo);
    }
    
    // Função para salvar os resultados em um arquivo
    void salvarResultados() const {
        ofstream arquivo("ranking.txt", ios::app); // Abre o arquivo em modo de anexo

        if (arquivo.is_open()) {
            arquivo << "Jogador: " << NomeJogador << " | Pontuação: " << pontuacao << " | Data/Hora: " << obterDataHoraAtual() << endl;
            arquivo.close();
        } else {
            cout << "Erro ao abrir o arquivo para salvar os resultados.\n";
        }
    }
    
    // Função para obter a data e hora atuais
    string obterDataHoraAtual() const {
        time_t agora = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&agora));
        return buffer;
    }
};


int main () {

    CampoMinado jogar;
    jogar.iniciarJogo();
    jogar.salvarResultados();

  return 0;
}