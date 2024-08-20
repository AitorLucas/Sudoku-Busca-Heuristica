# Relatório Métodos de Busca Heurística
**Estudante: Aitor Eler Lucas**

## Introdução

Este relatório apresenta uma análise de desempenho de quatro algoritmos utilizados para resolver o problema do Sudoku: Busca em Profundidade (DFS), Busca em Largura (BFS), Busca Gulosa e Busca A*. Os dados foram obtidos a partir da execução do código [`sudoku.cpp`](sudoku.cpp), que implementa todos os algoritmos e mede o tempo de execução e uso de memória para cada um deles.

## Descrição Geral do Problema

O Sudoku é um jogo de lógica que consiste em preencher uma grade 9x9 com números de 1 a 9, de forma que cada linha, coluna e cada uma das nove subgrades 3x3 contenham todos os números de 1 a 9 sem repetição. A implementação do código [`sudoku.cpp`](sudoku.cpp) inclui as seguintes funcionalidades principais:

1. **Leitura do Tabuleiro**: A função `lerSudoku` lê o tabuleiro de um arquivo de texto.
2. **Verificação de Segurança**: A função `eSeguro` verifica se é seguro colocar um número em uma posição específica.
3. **Impressão do Tabuleiro**: A função `imprimirSudoku` imprime o tabuleiro de Sudoku.
4. **Resolução do Sudoku**: A função `resolve` direciona a resolução para o algoritmo que será utilizado para obter o resultado e mede o tempo de execução dos algoritmos, podendo ser os algoritmos: `resolverSudokuDFS`, `resolverSudokuBFS`, `resolverSudokuGuloso` ou `resolverSudokuAEstrela`.
5. **Verificação da Solução**: A função `verificarSolucao` verifica se o Sudoku foi resolvido corretamente.
6. **Cálculos de Resultado**: As funções `usoDeMemoria`, `media` e `desvioPadrao` são usadas para retornar dados de resultado dos algoritmos.
7. **Contagem de Números Preenchidos**: As funções `contarNumerosLinha`, `contarNumerosColuna` e `contarNumerosSubgrade` contam quantos números já estão preenchidos em uma linha, coluna ou subgrade específica.
8. **Contagem de Candidatos Válidos**: A função `contarCandidatosValidos` conta quantos números válidos podem ser colocados em uma célula específica.
9. **Busca da Melhor Célula**: A função `encontrarCelulaComMenosCandidatos` encontra a célula com o menor número de candidatos válidos, utilizada na busca gulosa e na busca A*.

## Heurística Utilizada

A heurística utilizada nos algoritmos de Busca Gulosa e Busca A* é baseada na contagem de candidatos válidos para cada célula vazia do tabuleiro. A ideia é escolher a célula que possui o menor número de candidatos válidos, pois isso reduz o espaço de busca e aumenta a chance de encontrar uma solução mais rapidamente.

## Dados de Desempenho

Os dados de desempenho foram coletados a partir de 100 execuções de cada algoritmo. Os tempos de execução (em microssegundos) e o uso de memória (em KB) foram registrados e armazenados no arquivo [`resultados.csv`](resultados.csv). Abaixo estão os resultados resumidos:

| Dados                       | DFS       | BFS       | Guloso    | A*        |
|-----------------------------|-----------|-----------|-----------|-----------|
| Média Tempo (µs)            | 40.22     | 1176.57   | 121.71    | 327.72    |
| Desvio Padrão Tempo (µs)    | 85.6641   | 2509.93   | 64.6503   | 136.398   |
| Média Memória (KB)          | 1181      | 1182.4    | 1182.4    | 1182.72   |
| Desvio Padrão Memória (KB)  | 38.8613   | 37.5702   | 37.5702   | 37.6165   |

## Análise de Desempenho

### Tempo

#### Busca em Profundidade (DFS)

A Busca em Profundidade (DFS) é um algoritmo que explora o máximo possível cada ramo antes de retroceder. No contexto do Sudoku, o DFS tenta preencher cada célula vazia com um número válido e avança para a próxima célula. Se encontrar um impasse, ele retrocede e tenta um número diferente.

- **Média de Tempo de Execução**: 40.22 µs
- **Desvio Padrão**: 85.6641 µs

O DFS apresentou um tempo de execução médio relativamente baixo, indicando que, na maioria dos casos, ele consegue resolver o Sudoku rapidamente. No entanto, o desvio padrão relativamente alto (quando comparado ao valor da media) sugere que há variação significativa no tempo de execução dependendo da configuração inicial do tabuleiro.

#### Busca em Largura (BFS)

A Busca em Largura (BFS) é um algoritmo que explora todos os nós em um nível antes de passar para o próximo nível. No contexto do Sudoku, o BFS tenta preencher cada célula vazia com todos os números possíveis e avança para a próxima célula, explorando todas as possibilidades em paralelo.

- **Média de Tempo de Execução**: 1176.57 µs
- **Desvio Padrão**: 2509.93 µs

O BFS apresentou um tempo de execução médio significativamente maior em comparação com o DFS. Isso ocorre porque o BFS explora todas as possibilidades em paralelo, o que pode ser computacionalmente mais intensivo. O desvio padrão ainda maior indica que o tempo de execução do BFS é ainda mais sensível à configuração inicial do tabuleiro.

#### Busca Gulosa

A Busca Gulosa tenta preencher as células vazias com base em uma heurística que escolhe a célula com o menor número de candidatos válidos.

- **Média de Tempo de Execução**: 121.71 µs
- **Desvio Padrão**: 64.6503 µs

A Busca Gulosa apresentou um tempo de execução médio maior que o DFS, mas ainda significativamente menor que o BFS. O desvio padrão menor indica uma variação menor no tempo de execução.

#### Busca A*

Na Busca A*, a função de custo `f(n) = g(n) + h(n)` é utilizada, onde:
- `g(n)` é o custo do caminho desde o início até o nó atual.
- `h(n)` é a heurística que estima o custo do nó atual até o objetivo.

A heurística `h(n)` é calculada como a soma do número de candidatos válidos para todas as células vazias. A célula com o menor número de candidatos válidos é escolhida para ser preenchida, e o processo continua até que o tabuleiro esteja completo ou não haja mais candidatos válidos.

Essa abordagem heurística é eficaz porque reduz o espaço de busca e prioriza as células que são mais restritivas, aumentando a chance de encontrar uma solução válida mais rapidamente.

- **Média de Tempo de Execução**: 327.72 µs
- **Desvio Padrão**: 136.398 µs

A Busca A* apresentou um tempo de execução médio maior que o DFS e a Busca Gulosa, mas menor que o BFS. O desvio padrão indica uma variação moderada no tempo de execução. Contrariando o que se esperava, ficando mais lenta que a Busca Gulosa.

### Memória

**Comparação Geral**

Os dados obtidos para memória foram muito próximos nos quatro algoritmos, o que implica que talvez o método de obtenção desses dados não esteja mostrando dados muito precisos. De uma maneira geral, os algoritmos se comportam da seguinte forma:

- **DFS**: Geralmente, o DFS tende a usar menos memória do que o BFS, pois ele explora um caminho até o fim antes de retroceder, mantendo menos estados intermediários na memória ao mesmo tempo.
- **BFS**: O BFS tende a usar mais memória do que o DFS, pois ele mantém todos os estados intermediários de um nível na memória antes de passar para o próximo nível. Isso pode levar a um uso significativo de memória em tabuleiros complexos.
- **Guloso e A***: O uso de memória desses algoritmos pode variar dependendo da implementação específica da heurística e da função de custo.

## Dados Iniciais

Os dados iniciais são gerados pelo código [`sudoku_generator.cpp`](sudoku_generator.cpp). Este código cria tabuleiros de Sudoku com um número variável de elementos vazios, que são então utilizados para testar os algoritmos DFS, BFS, Busca Gulosa e Busca A*. A geração dos tabuleiros é feita de forma aleatória, garantindo uma variedade de configurações iniciais para os testes.