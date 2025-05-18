# Super Trunfo - Desafio de Lógica Avançado em C

Este projeto é uma implementação em C de uma rodada do jogo Super Trunfo, focada na comparação de atributos entre duas cartas de países. O jogador escolhe dois atributos diferentes, e a carta com a maior soma dos valores desses dois atributos vence a rodada.

## Funcionalidades

* **Cartas Pré-definidas:** O jogo utiliza duas cartas de países com dados pré-definidos (Brasil e Japão, como exemplo).
* **Seleção Dinâmica de Atributos:** O jogador escolhe interativamente dois atributos distintos para a comparação. O menu para o segundo atributo não lista o primeiro atributo já selecionado.
* **Lógica de Comparação:** A comparação final é baseada na soma dos valores dos dois atributos escolhidos para cada carta.
* **Tratamento de Empates:** O sistema identifica e anuncia empates.
* **Exibição Clara:** Os resultados, incluindo os atributos escolhidos, seus valores para cada carta e a soma, são exibidos de forma organizada.
* **Uso de Estruturas de Decisão:** O código emprega `if-else`, `switch` e o operador ternário para controlar o fluxo e tomar decisões.

## Atributos Disponíveis para Comparação

Ao selecionar os atributos, você verá um menu numerado. Os atributos e seus respectivos campos na estrutura de dados são:

1.  **População** (`populacao`): Número total de habitantes.
2.  **Área (km²)** (`area`): Extensão territorial em quilômetros quadrados.
3.  **PIB (Trilhões USD)** (`pib`): Produto Interno Bruto, em trilhões de dólares americanos (ex: 1.6 para 1,6 trilhões).
4.  **Pontos Turísticos** (`numPontosTuristicos`): Quantidade de pontos turísticos notáveis.
5.  **Densidade Populacional (hab/km²)** (`densidadePopulacional`): Habitantes por quilômetro quadrado. *Observação: Na regra clássica do Super Trunfo, menor densidade é melhor, mas para a SOMA DE ATRIBUTOS deste desafio, o valor numérico da densidade é usado diretamente na soma.*
6.  **PIB per Capita (USD)** (`pibPerCapita`): Produto Interno Bruto dividido pela população.

## Como Compilar

### Pré-requisitos

* Um compilador C (como GCC, Clang, ou o compilador MSVC incluído no Visual Studio).

### Usando GCC (Linux, macOS, MinGW no Windows)

Abra um terminal ou prompt de comando, navegue até o diretório onde você salvou o arquivo `super_trunfo_avancado.c` (ou o nome que você deu) e execute o seguinte comando:

```bash
gcc nome_do_arquivo.c -o super_trunfo_jogo