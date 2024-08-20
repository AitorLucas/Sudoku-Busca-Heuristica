#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <cmath>
#include <getopt.h>
#include <windows.h>
#include <psapi.h>

using namespace std;

const int N = 9; // Tamanho do tabuleiro do Sudoku

enum Algoritmo {
    DFS,
    BFS,
    Guloso,
    AEstrela
}; // Algoritimos utilizados para resolver o Sudoku

// Função para imprimir o tabuleiro de Sudoku
void imprimirSudoku(const vector<vector<int>>& tabuleiro) {
    for (int i = 0; i < N; i++) {
        if (i % 3 == 0) {
            cout << "+-------+-------+-------+" << endl;
        }
        for (int j = 0; j < N; j++) {
            if (j % 3 == 0) {
                cout << "| ";
            }
            cout << tabuleiro[i][j] << " ";
            if (j == N - 1) {
                cout << "|";
            }
        }
        cout << endl;
    }
    cout << "+-------+-------+-------+" << endl;
    cout << endl;
}

// Função para verificar se é seguro colocar um número em uma posição específica
bool eSeguro(const vector<vector<int>>& tabuleiro, int linha, int coluna, int num) {
    // Verifica se o número está presente na linha ou coluna
    for (int x = 0; x < N; x++) {
        if (tabuleiro[linha][x] == num || tabuleiro[x][coluna] == num) { 
            return false;
        }
    }
    
    // Verifica se o número está presente no quadrado 3x3
    int startRow = linha - linha % 3, startCol = coluna - coluna % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[i + startRow][j + startCol] == num) {
                return false;
            }
        }
    }

    return true; // Devolve True se o número não estiver presente na linha, coluna ou quadrado 3x3
}

// Função de busca em profundidade (DFS) para resolver o Sudoku
bool resolverSudokuDFS(vector<vector<int>>& tabuleiro) {
    int linha = -1, coluna = -1;
    bool vazio = false;
    
    // Encontra uma célula vazia
    for (int i = 0; i < N && !vazio; i++) {
        for (int j = 0; j < N && !vazio; j++) {
            if (tabuleiro[i][j] == 0) {
                linha = i;
                coluna = j;
                vazio = true;
            }
        }
    }
    
    // Se não há células vazias, o Sudoku está resolvido
    if (!vazio) {
        return true; 
    }

    // Tenta números de 1 a 9 na célula vazia
    for (int num = 1; num <= 9; num++) {
        if (eSeguro(tabuleiro, linha, coluna, num)) { // Verifica se o número é seguro para ser colocado na célula vazia (ou seja, não está presente na linha, coluna e quadrado 3x3)
            tabuleiro[linha][coluna] = num; // Atribui o número à célula vazia
            if (resolverSudokuDFS(tabuleiro)) { // Chamada da função recursiva para resolver as outras células, se True, o Sudoku está resolvido
                return true;
            }
            tabuleiro[linha][coluna] = 0; // Se a atribuição do número não levar a uma solução, a célula é redefinida para 0
        }
    }

    return false; // Retorna False se não houver solução para a célula vazia. Indicando que não é possível resolver o Sudoku
}

// Função de busca em largura (BFS) para resolver o Sudoku
bool resolverSudokuBFS(vector<vector<int>>& tabuleiro) {
    queue<vector<vector<int>>> fila;
    fila.push(tabuleiro);
    
    while (!fila.empty()) {
        vector<vector<int>> curr = fila.front();
        fila.pop();
        
        int linha = -1, coluna = -1;
        bool vazio = false;
        
        // Encontra uma célula vazia
        for (int i = 0; i < N && !vazio; i++) {
            for (int j = 0; j < N && !vazio; j++) {
                if (curr[i][j] == 0) {
                    linha = i;
                    coluna = j;
                    vazio = true;
                }
            }
        }
        
        // Se não há células vazias, o Sudoku está resolvido
        if (!vazio) {
            tabuleiro = curr;
            return true; 
        }
        
        // Tenta números de 1 a 9 na célula vazia
        for (int num = 1; num <= 9; num++) { 
            if (eSeguro(curr, linha, coluna, num)) { // Verifica se o número é seguro para ser colocado na célula vazia (ou seja, não está presente na linha, coluna e quadrado 3x3)
                vector<vector<int>> novoTabuleiro = curr; // Cria uma cópia do tabuleiro atual
                novoTabuleiro[linha][coluna] = num; // Atribui o número à célula vazia nesse novo tabuleiro
                fila.push(novoTabuleiro); // Adiciona o novo tabuleiro à fila
            }
        }
    }

    return false;
}

// Função para ler o Sudoku de um arquivo .txt
vector<vector<int>> lerSudoku(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    vector<vector<int>> tabuleiro(N, vector<int>(N, 0));
    
    if (arquivo.is_open()) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                arquivo >> tabuleiro[i][j];
            }
        }
        arquivo.close();
    } else {
        cout << "Não foi possível abrir o arquivo." << endl;
    }
    
    return tabuleiro;
}

// Função para verificar se o Sudoku está resolvido corretamente
bool verificarSolucao(const vector<vector<int>>& tabuleiro) {
    const set<int> numeros = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    // testa as linhas do tabuleiro do sudoku
    for (int i = 0; i < N; i++) {
        set<int> linha(tabuleiro[i].begin(), tabuleiro[i].end());
        if (linha != numeros) {
            return false;
        }
    }

    // testa as colunas do tabuleiro do sudoku
    for (int i = 0; i < N; i++) {
        set<int> coluna;
        for (int j = 0; j < N; j++) {
            coluna.insert(tabuleiro[j][i]);
        }
        if (coluna != numeros) {
            return false;
        }
    }

    // testa os quadrados do tabuleiro do sudoku
    for (int i = 0; i < N; i += 3) {
        for (int j = 0; j < N; j += 3) {
            set<int> quadrado;
            for (int k = i; k < i + 3; k++) {
                for (int l = j; l < j + 3; l++) {
                    quadrado.insert(tabuleiro[k][l]);
                }
            }
            if (quadrado != numeros) {
                return false;
            }
        }
    }

    return true;
}

// Funcao que chama o algoritmo de resolucao do sudoku e retorna o tempo de execucao (-1 para error)
int resolve(vector<vector<int>> &tabuleiro, bool (*resolverSudoku)(vector<vector<int>>&), bool imprimir, Algoritmo algoritmo) {
    int duracao = -1;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    string resultadoDoAlgoritimo = "XXXXXXX"; // Se o algoritmo não resolver o Sudoku, o resultado será XXXXXXX
    if (resolverSudoku(tabuleiro)) {
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        duracao = chrono::duration_cast<chrono::microseconds>(end - begin).count();

        if (verificarSolucao(tabuleiro)) {
            resultadoDoAlgoritimo = " OK"; // Se o Sudoku foi resolvido corretamente, o resultado será OK
        } else {
            resultadoDoAlgoritimo = "NOK"; // Se o Sudoku foi resolvido incorretamente, o resultado será NOK
        }
    }

    // Imprimir resultados
    if (imprimir) {
        switch (algoritmo) {
        case DFS:
            cout << "DFS: " << '\t';
            break;
        case BFS:
            cout << "BFS: " << '\t';
            break;
        case Guloso:
            cout << "Guloso: ";
            break;
        case AEstrela:
            cout << "A*: " << '\t';
            break;
        }

        if (duracao != -1) {
            cout << duracao << " microssegundos" << endl;
        }
        cout << "Resultado: " << resultadoDoAlgoritimo << endl;
        // imprimirSudoku(tabuleiro); // IMPRIMIR TABULEIRO RESOLVIDO
    }

    return duracao;
}

// Função para medir o uso de memória
float usoDeMemoria() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return static_cast<float>(pmc.PrivateUsage) / 1024.0f; // Converte bytes para KB
}

// Funcao que calculo a media
float media(vector<float> vetor) {
    float soma = 0;
    for (int elemento : vetor) {
        soma += elemento;
    }
    return soma / vetor.size();
}

// Funcao que calcula o desvio padrao
float desvioPadrao(vector<float> vetor, float media) {
    if (vetor.empty()) {
        return 0; // Retorna 0 se o vetor estiver vazio
    }

    float soma = 0;
    for (int elemento : vetor) {
        soma += pow(elemento - media, 2);
    }
    return sqrt(soma / vetor.size());
}

// Função para contar números preenchidos em uma linha
int contarNumerosLinha(const vector<vector<int>>& tabuleiro, int linha) {
    int count = 0;
    for (int num : tabuleiro[linha]) {
        if (num != 0) {
            count++;
        }
    }
    return count;
}

// Função para contar números preenchidos em uma coluna
int contarNumerosColuna(const vector<vector<int>>& tabuleiro, int coluna) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        if (tabuleiro[i][coluna] != 0) {
            count++;
        }
    }
    return count;
}

// Função para contar números preenchidos em uma subgrade
int contarNumerosSubgrade(const vector<vector<int>>& tabuleiro, int startRow, int startCol) {
    int count = 0;
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            if (tabuleiro[i][j] != 0) {
                count++;
            }
        }
    }
    return count;
}

// Função para contar candidatos válidos em uma célula
int contarCandidatosValidos(const vector<vector<int>>& tabuleiro, int linha, int coluna) {
    if (tabuleiro[linha][coluna] != 0) {
        return 0; // Célula já preenchida
    }

    bool candidatos[10] = {false }; // Candidatos válidos de 1 a 9

    // Verificar linha
    for (int i = 0; i < 9; i++) {
        if (tabuleiro[linha][i] != 0) {
            candidatos[tabuleiro[linha][i]] = true;
        }
    }

    // Verificar coluna
    for (int i = 0; i < 9; i++) {
        if (tabuleiro[i][coluna] != 0) {
            candidatos[tabuleiro[i][coluna]] = true;
        }
    }

    // Verificar subgrade
    int subgradeLinha = (linha / 3) * 3;
    int subgradeColuna = (coluna / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (tabuleiro[subgradeLinha + i][subgradeColuna + j] != 0) {
                candidatos[tabuleiro[subgradeLinha + i][subgradeColuna + j]] = true;
            }
        }
    }

    // Contar candidatos válidos
    int count = 0;
    for (int i = 1; i <= 9; i++) {
        if (!candidatos[i]) {
            count++;
        }
    }

    return count;
}

// Função para encontrar a célula com menos candidatos válidos ({-1, -1} para célula não encontrada)
pair<int, int> encontrarCelulaComMenosCandidatos(const vector<vector<int>>& tabuleiro) {
    int minCandidatos = 10; // Maior que o número máximo de candidatos possíveis (9)
    pair<int, int> melhorCelula = {-1, -1};

    for (int linha = 0; linha < 9; linha++) { 
        for (int coluna = 0; coluna < 9; coluna++) {
            if (tabuleiro[linha][coluna] == 0) { // Célula vazia
                int candidatos = contarCandidatosValidos(tabuleiro, linha, coluna); // Contar candidatos válidos
                if (candidatos < minCandidatos) {   // Atualizar a célula com menos candidatos
                    minCandidatos = candidatos;     // Atualizar o número mínimo de candidatos
                    melhorCelula = {linha, coluna}; // Atualizar a célula com menos candidatos
                }
            }
        }
    }

    return melhorCelula;
}

// Função de busca gulosa para resolver o Sudoku
bool resolverSudokuGuloso(vector<vector<int>>& tabuleiro) {
    pair<int, int> melhorCelula = encontrarCelulaComMenosCandidatos(tabuleiro);

    if (melhorCelula.first == -1 && melhorCelula.second == -1) {
        return true; // Sudoku resolvido
    }

    int linha = melhorCelula.first;
    int coluna = melhorCelula.second;

    for (int num = 1; num <= 9; num++) {
        if (eSeguro(tabuleiro, linha, coluna, num)) {   // Verifica se o número é seguro para ser colocado na célula vazia (ou seja, não está presente na linha, coluna e quadrado 3x3)
            tabuleiro[linha][coluna] = num;             // Atribui o número à célula vazia
            if (resolverSudokuGuloso(tabuleiro)) {      // Chamada da função recursiva para resolver as outras células, se True, o Sudoku está resolvido
                return true;    // Sudoku resolvido
            }
            tabuleiro[linha][coluna] = 0;               // Se a atribuição do número não levar a uma solução, a célula é redefinida para 0
        }
    }

    return false;
}

// Função de custo f(n) = g(n) + h(n)
int funcaoCusto(const vector<vector<int>>& tabuleiro, int g) {
    int h = 0;
    for (int i = 0; i < N; i++) {
        h += contarNumerosLinha(tabuleiro, i);  // Contar números preenchidos em uma linha
        h += contarNumerosColuna(tabuleiro, i); // Contar números preenchidos em uma coluna
    }
    for (int i = 0; i < N; i += 3) {
        for (int j = 0; j < N; j += 3) {
            h += contarNumerosSubgrade(tabuleiro, i, j);    // Contar números preenchidos em uma subgrade
        }
    }
    return g + h;
}

// Função de busca A* para resolver o Sudoku
bool resolverSudokuAEstrela(vector<vector<int>>& tabuleiro) {
    priority_queue<tuple<int, int, vector<vector<int>>>> pq; // Fila de prioridade para armazenar o custo, g(n) e o estado do tabuleiro
    pq.push(make_tuple(0, 0, tabuleiro));

    while (!pq.empty()) {
        auto [custo, g, estado] = pq.top(); // Obter o estado com menor custo
        pq.pop();

        pair<int, int> melhorCelula = encontrarCelulaComMenosCandidatos(estado);    // Encontrar a célula com menos candidatos válidos

        if (melhorCelula.first == -1 && melhorCelula.second == -1) {    // Sudoku resolvido
            tabuleiro = estado;
            return true;
        }

        int linha = melhorCelula.first;     // Linha da célula com menos candidatos
        int coluna = melhorCelula.second;   // Coluna da célula com menos candidatos

        for (int num = 1; num <= 9; num++) {
            if (eSeguro(estado, linha, coluna, num)) {              // Verifica se o número é seguro para ser colocado na célula vazia (ou seja, não está presente na linha, coluna e quadrado 3x3)
                vector<vector<int>> novoEstado = estado;            // Cria uma cópia do estado atual
                novoEstado[linha][coluna] = num;                    // Atribui o número à célula vazia
                int novoCusto = funcaoCusto(novoEstado, g + 1);     // Calcula o custo f(n) = g(n) + h(n)
                pq.push(make_tuple(novoCusto, g + 1, novoEstado));  // Adiciona o novo estado à fila de prioridade
            }
        }
    }

    return false;
}

//  MAIN
//
// Parametros:
// -i: Imprimir tabuleiros e resultados
// -t: Imprimir tempo de execucao
int main(int argc, char *argv[]) {
    int numeroDeTestes = 100;
    vector<float> temposDFS;
    vector<float> temposBFS;
    vector<float> temposGuloso;
    vector<float> temposAEstrela;
    vector<float> memoriaDFS(numeroDeTestes);
    vector<float> memoriaBFS(numeroDeTestes);
    vector<float> memoriaGuloso(numeroDeTestes);
    vector<float> memoriaAEstrela(numeroDeTestes);
    
    // Processa argumentos da linha de comando
    bool imprimir = false;
    bool imprimirTempo = false;
    int opt;
    while ((opt = getopt(argc, argv, "it")) != -1) {
        switch (opt) {
            case 'i':
                imprimir = true;
                break;
            case 't':
                imprimirTempo = true;
                break;
            default:
                cerr << "Uso: " << argv[0] << " [-i] [-t]" << endl;
                return 1;
        }
    }

    // Executa os testes de Sudoku na pasta testes
    for (int teste = 1; teste <= numeroDeTestes; teste++) {
        string name = "testes/" + to_string(teste) + ".txt";
        vector<vector<int>> tabuleiro = lerSudoku(name);
        
        //  Imprimir o tabuleiro de Sudoku
        if (imprimir || imprimirTempo) {
            cout << "=========================" << endl;
            cout << "\tTeste " << teste << endl;
            cout << "=========================" << endl;
        }
        if (imprimir) {
            imprimirSudoku(tabuleiro);
        }

        vector<vector<int>> tabuleiroDFS = tabuleiro;
        vector<vector<int>> tabuleiroBFS = tabuleiro;
        vector<vector<int>> tabuleiroGuloso = tabuleiro;
        vector<vector<int>> tabuleiroAEstrela = tabuleiro;

        int tempoResolucaoDFS = resolve(tabuleiroDFS, resolverSudokuDFS, imprimirTempo, DFS);
        if (tempoResolucaoDFS != -1) {
            temposDFS.push_back(tempoResolucaoDFS);
        }
        memoriaDFS[teste - 1] = usoDeMemoria();

        int tempoResolucaoBFS = resolve(tabuleiroBFS, resolverSudokuBFS, imprimirTempo, BFS);
        if (tempoResolucaoBFS != -1) {
            temposBFS.push_back(tempoResolucaoBFS);
        }
        memoriaBFS[teste - 1] = usoDeMemoria();

        int tempoResolucaoGuloso = resolve(tabuleiroGuloso, resolverSudokuGuloso, imprimirTempo, Guloso);
        if (tempoResolucaoGuloso != -1) {
            temposGuloso.push_back(tempoResolucaoGuloso);
        }
        memoriaGuloso[teste - 1] = usoDeMemoria();

        int tempoResolucaoAEstrela = resolve(tabuleiroAEstrela, resolverSudokuAEstrela, imprimirTempo, AEstrela);
        if (tempoResolucaoAEstrela != -1) {
            temposAEstrela.push_back(tempoResolucaoAEstrela);
        }
        memoriaAEstrela[teste - 1] = usoDeMemoria();
    }

    // Calculo dos resultados de tempo
    float mediaTempoDFS = media(temposDFS);
    float mediaTempoBFS = media(temposBFS);
    float mediaTempoGuloso = media(temposGuloso);
    float mediaTempoAEstrela = media(temposAEstrela);
    float desvioTempoDFS = desvioPadrao(temposDFS, mediaTempoDFS);
    float desvioTempoBFS = desvioPadrao(temposBFS, mediaTempoBFS);
    float desvioTempoGuloso = desvioPadrao(temposGuloso, mediaTempoGuloso);
    float desvioTempoAEstrela = desvioPadrao(temposAEstrela, mediaTempoAEstrela);
    // Calculo dos resultados de memoria
    float mediaMemoriaDFS = media(memoriaDFS);
    float mediaMemoriaBFS = media(memoriaBFS);
    float mediaMemoriaGuloso = media(memoriaGuloso);
    float mediaMemoriaAEstrela = media(memoriaAEstrela);
    float desvioMemoriaDFS = desvioPadrao(memoriaDFS, mediaMemoriaDFS);
    float desvioMemoriaBFS = desvioPadrao(memoriaBFS, mediaMemoriaBFS);
    float desvioMemoriaGuloso = desvioPadrao(memoriaGuloso, mediaMemoriaGuloso);
    float desvioMemoriaAEstrela = desvioPadrao(memoriaAEstrela, mediaMemoriaAEstrela);

    // Imprime resultados
    cout << endl;
    cout << "==================================================" << endl;
    cout << " Media tempo DFS: " << mediaTempoDFS << " microssegundos" << endl;
    cout << " Desvio padrao tempo DFS: " << desvioTempoDFS << " microssegundos" << endl;
    cout << " Media memoria DFS: " << mediaMemoriaDFS << " KB" << endl;
    cout << " Desvio padrao memoria DFS: " << desvioMemoriaDFS << " KB" << endl;
    cout << endl;
    cout << " Media tempo BFS: " << mediaTempoBFS << " microssegundos" << endl;
    cout << " Desvio padrao tempo BFS: " << desvioTempoBFS << " microssegundos" << endl;
    cout << " Media memoria BFS: " << mediaMemoriaBFS << " KB" << endl;
    cout << " Desvio padrao memoria BFS: " << desvioMemoriaBFS << " KB" << endl;
    cout << endl;
    cout << " Media tempo Guloso: " << mediaTempoGuloso << " microssegundos" << endl;
    cout << " Desvio padrao tempo Guloso: " << desvioTempoGuloso << " microssegundos" << endl;
    cout << " Media memoria Guloso: " << mediaMemoriaGuloso << " KB" << endl;
    cout << " Desvio padrao memoria Guloso: " << desvioMemoriaGuloso << " KB" << endl;
    cout << endl;
    cout << " Media tempo AEstrela: " << mediaTempoAEstrela << " microssegundos" << endl;
    cout << " Desvio padrao tempo AEstrela: " << desvioTempoAEstrela << " microssegundos" << endl;
    cout << " Media memoria AEstrela: " << mediaMemoriaAEstrela << " KB" << endl;
    cout << " Desvio padrao memoria AEstrela: " << desvioMemoriaAEstrela << " KB" << endl;
    cout << "==================================================" << endl;
    cout << endl;

    // Cria um arquivo de saida com os resultados (CSV)
    ofstream arquivoCSV("resultados.csv");
    if(arquivoCSV.is_open()) {
        // Cabeçalho do arquivo CSV
        arquivoCSV << "Dados,DFS Tempo(microssegundos),BFS Tempo(microssegundos),Guloso Tempo(microssegundos),AEstrela Tempo(microssegundos),DFS Memoria(KB),BFS Memoria(KB),Guloso Memoria(KB), AEstrela Memoria(KB)\n";
        
        // Escrever tempos de execução e uso de memória
        size_t maxSize = max(temposDFS.size(), temposBFS.size());
        for (size_t i = 0; i < 100; ++i) {
            arquivoCSV << "Teste " << i + 1 << ",";
            if (i < temposDFS.size()) {
                arquivoCSV << temposDFS[i];
            }
            arquivoCSV << ",";
            if (i < temposBFS.size()) {
                arquivoCSV << temposBFS[i];
            }
            arquivoCSV << ",";
            if (i < temposGuloso.size()) {
                arquivoCSV << temposGuloso[i];
            }
            arquivoCSV << ",";
            if (i < temposAEstrela.size()) {
                arquivoCSV << temposAEstrela[i];
            }
            arquivoCSV << ",";
            if (i < memoriaDFS.size()) {
                arquivoCSV << memoriaDFS[i];
            }
            arquivoCSV << ",";
            if (i < memoriaBFS.size()) {
                arquivoCSV << memoriaBFS[i];
            }
            arquivoCSV << ",";
            if (i < memoriaGuloso.size()) {
                arquivoCSV << memoriaGuloso[i];
            }
            arquivoCSV << ",";
            if (i < memoriaAEstrela.size()) {
                arquivoCSV << memoriaAEstrela[i];
            }
            arquivoCSV << "\n";
        }
        
        // Escrever média e desvio padrão
        arquivoCSV << "Tempo Medio," << mediaTempoDFS << "," << mediaTempoBFS << "," << mediaTempoGuloso << "," << mediaTempoAEstrela << "\n";
        arquivoCSV << "Tempo Desvio Padrao," << desvioTempoDFS << "," << desvioTempoBFS << "," << desvioTempoGuloso << "," << desvioTempoAEstrela << "\n";
        arquivoCSV << "Memoria Media," << ",,,," << mediaMemoriaDFS << "," << mediaMemoriaBFS << "," << mediaMemoriaGuloso << "," << mediaMemoriaAEstrela << "\n";
        arquivoCSV << "Memoria Desvio Padrao," << ",,,," << desvioMemoriaDFS << "," << desvioMemoriaBFS << "," << desvioMemoriaGuloso << "," << desvioMemoriaAEstrela << "\n";

        // Fechar o arquivo
        arquivoCSV.close();
        cout << "Resultados salvos em resultados.csv" << endl;
    } else {
        cerr<< "Erro ao abrir o arquivo resultados.csv" << endl;
        return -1;
    }

    cout << endl << "[POSSIVEIS OPCOES DE EXECUCAO]" << endl;
    cout <<  "-i" << '\t' << "Imprimir tabuleiros" << endl;
    cout <<  "-t" << '\t' << "Imprimir tempo de execucao" << endl;
    cout << endl;

    return 0;
}
