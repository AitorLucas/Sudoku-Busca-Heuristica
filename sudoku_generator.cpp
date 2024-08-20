#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <filesystem> // C++17 or later
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 9; // Tamanho do tabuleiro do Sudoku

// Função para imprimir o tabuleiro de Sudoku
void imprimirSudoku(const vector<vector<int>>& tabuleiro, ostream& out) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            out << tabuleiro[i][j] << " ";
        }
        out << endl;
    }
}

// Função para verificar se é seguro colocar um número em uma posição específica
bool eSeguro(const vector<vector<int>>& tabuleiro, int linha, int coluna, int num) {
    for (int x = 0; x < N; x++) {
        if (tabuleiro[linha][x] == num || tabuleiro[x][coluna] == num) {
            return false;
        }
    }
    
    int startRow = linha - linha % 3, startCol = coluna - coluna % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true;
}

// Função para resolver o Sudoku usando backtracking
bool resolverSudoku(vector<vector<int>>& tabuleiro) {
    for (int linha = 0; linha < N; linha++) {
        for (int coluna = 0; coluna < N; coluna++) {
            if (tabuleiro[linha][coluna] == 0) {
                for (int num = 1; num <= N; num++) {
                    if (eSeguro(tabuleiro, linha, coluna, num)) {
                        tabuleiro[linha][coluna] = num;
                        if (resolverSudoku(tabuleiro)) {
                            return true;
                        }
                        tabuleiro[linha][coluna] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// Função para preencher o tabuleiro de Sudoku aleatoriamente de forma válida
void preencherAleatoriamente(vector<vector<int>>& tabuleiro) {
    vector<int> numeros(N);
    iota(numeros.begin(), numeros.end(), 1);
    random_device rd;
    mt19937 g(rd());

    // Preencher a diagonal de subgrades 3x3
    for (int i = 0; i < N; i += 3) {
        shuffle(numeros.begin(), numeros.end(), g);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                tabuleiro[i + j][i + k] = numeros[j * 3 + k];
            }
        }
    }

    // Resolver o tabuleiro parcialmente preenchido
    resolverSudoku(tabuleiro);
}

// Função para remover elementos do tabuleiro de Sudoku
void removerElementos(vector<vector<int>>& tabuleiro, int numElementos) {
    random_device rd;
    mt19937 g(rd());
    uniform_int_distribution<> dist(0, N - 1);

    while (numElementos > 0) {
        int i = dist(g);
        int j = dist(g);
        if (tabuleiro[i][j] != 0) {
            int backup = tabuleiro[i][j];
            tabuleiro[i][j] = 0;
            vector<vector<int>> copia = tabuleiro;
            if (resolverSudoku(copia)) {
                numElementos--;
            } else {
                tabuleiro[i][j] = backup;
            }
        }
    }
}

// Função para gerar um tabuleiro de Sudoku completo com elementos vazios
vector<vector<int>> gerarSudoku(int numElementosVazios) {
    vector<vector<int>> tabuleiro(N, vector<int>(N, 0));
    preencherAleatoriamente(tabuleiro);
    resolverSudoku(tabuleiro);
    removerElementos(tabuleiro, numElementosVazios);
    return tabuleiro;
}

int main() {
    // Cria a pasta "testes" se não existir
    filesystem::create_directory("testes");

    // Seed do gerador de numeros aleatorios
    srand(static_cast<unsigned int>(time(0)));

    for (int k = 1; k <= 100; k++) {
        // Define o número de elementos vazios entre 15 e 45
        int numElementosVazios = rand() % 31 + 15;
        cout << "Teste " << k << ": " << numElementosVazios << " elementos vazios" << endl;
        
        // Gera o tabuleiro de Sudoku
        vector<vector<int>> tabuleiro = gerarSudoku(numElementosVazios);

        // Nome do arquivo
        string nomeArquivo = "testes/" + to_string(k) + ".txt";

        // Abre o arquivo para escrita
        ofstream arquivo(nomeArquivo);

        // Verifica se o arquivo foi aberto corretamente
        if (arquivo.is_open()) {
            // Imprime o tabuleiro no arquivo
            imprimirSudoku(tabuleiro, arquivo);
            arquivo.close();
        } else {
            cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
            return -1;
        }
    }

    return 0;
}