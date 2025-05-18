#define _CRT_SECURE_NO_WARNINGS // Necessário para o Visual Studio desabilitar avisos de scanf, etc.
#include <stdio.h>              // Para funções de entrada/saída padrão (printf, scanf, getchar)
#include <string.h>             // Para funções de manipulação de strings (strcpy, strcmp, etc. - aqui usamos para sprintf)
#include <stdlib.h>             // Para funções utilitárias gerais (system - não usado ativamente para manter portabilidade)

// --- Constantes para os IDs dos Atributos ---
// Estes defines facilitam a leitura e manutenção do código ao referenciar atributos.
#define ATTR_POPULACAO 1
#define ATTR_AREA 2
#define ATTR_PIB 3
#define ATTR_PONTOS_TURISTICOS 4
#define ATTR_DENSIDADE 5
#define ATTR_PIB_PER_CAPITA 6
#define NUM_ATRIBUTOS_TOTAL 6 // Total de atributos disponíveis para seleção

// --- Estrutura da Carta Super Trunfo ---
// Define os campos de dados para cada carta do jogo.
typedef struct {
    char nomePais[50];              // Nome do país representado pela carta
    unsigned long int populacao;    // População do país (unsigned long int para números grandes)
    double area;                    // Área do país em km²
    double pib;                     // Produto Interno Bruto (ex: em trilhões de USD)
    int numPontosTuristicos;        // Número de pontos turísticos relevantes
    double densidadePopulacional;   // Calculado: populacao / area (hab/km²)
    double pibPerCapita;            // Calculado: pib / populacao (USD)
} CartaSuperTrunfo;

// --- Protótipos das Funções ---
// Declarações antecipadas das funções utilizadas no programa.
void limparBufferEntrada();
void calcularAtributosDerivados(CartaSuperTrunfo* carta);
const char* obterNomeAtributo(int idAtributo);
double obterValorAtributo(const CartaSuperTrunfo* carta, int idAtributo);
int selecionarAtributo(int atributoJaEscolhido, const char* textoPrompt);

/**
 * @brief Limpa o buffer de entrada do teclado.
 *
 * Consome caracteres restantes no buffer de entrada até encontrar uma nova linha ('\n')
 * ou o final do arquivo (EOF). Essencial após usar scanf para ler números, antes
 * de ler strings com fgets ou para evitar que entradas residuais afetem leituras futuras.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Calcula os atributos derivados de uma carta (Densidade Populacional e PIB per Capita).
 *
 * @param carta Ponteiro para a struct CartaSuperTrunfo cujos atributos derivados serão calculados.
 * Os campos 'densidadePopulacional' e 'pibPerCapita' da struct serão atualizados.
 */
void calcularAtributosDerivados(CartaSuperTrunfo* carta) {
    // Calcula a densidade populacional, evitando divisão por zero se a área for 0.
    if (carta->area > 0) {
        carta->densidadePopulacional = (double)carta->populacao / carta->area;
    }
    else {
        // Se a área for 0, a densidade é considerada 0 (ou poderia ser um valor indicando indefinição).
        // Para a regra de "menor densidade é melhor", 0 é o melhor valor possível.
        carta->densidadePopulacional = 0.0;
    }

    // Calcula o PIB per capita, evitando divisão por zero se a população for 0.
    if (carta->populacao > 0) {
        // Exemplo: se o PIB está em trilhões (10^12) na struct, ajusta para a unidade correta.
        carta->pibPerCapita = (carta->pib * 1e12) / (double)carta->populacao;
    }
    else {
        carta->pibPerCapita = 0.0;
    }
}

/**
 * @brief Retorna o nome de um atributo como string, dado o seu ID numérico.
 *
 * @param idAtributo O ID numérico do atributo (conforme definido pelas constantes ATTR_*).
 * @return const char* Uma string constante contendo o nome do atributo.
 * Retorna "Desconhecido" se o ID não for válido.
 */
const char* obterNomeAtributo(int idAtributo) {
    // Estrutura switch para mapear o ID do atributo ao seu nome textual.
    switch (idAtributo) {
    case ATTR_POPULACAO: return "Populacao";
    case ATTR_AREA: return "Area (km2)";
    case ATTR_PIB: return "PIB (Trilhoes USD)";
    case ATTR_PONTOS_TURISTICOS: return "Pontos Turisticos";
    case ATTR_DENSIDADE: return "Densidade Populacional (hab/km2)";
    case ATTR_PIB_PER_CAPITA: return "PIB per Capita (USD)";
    default: return "Desconhecido"; // Caso o ID não corresponda a nenhum atributo conhecido.
    }
}

/**
 * @brief Obtém o valor numérico de um atributo específico de uma carta.
 *
 * @param carta Ponteiro constante para a struct CartaSuperTrunfo da qual o valor será extraído.
 * @param idAtributo O ID numérico do atributo desejado.
 * @return double O valor do atributo. Retorna 0.0 se o ID do atributo for inválido.
 * Todos os valores são retornados como double para facilitar cálculos consistentes.
 */
double obterValorAtributo(const CartaSuperTrunfo* carta, int idAtributo) {
    // Estrutura switch para retornar o valor do atributo correto da carta.
    switch (idAtributo) {
    case ATTR_POPULACAO: return (double)carta->populacao; // Converte para double
    case ATTR_AREA: return carta->area;
    case ATTR_PIB: return carta->pib; // Valor direto (ex: em trilhões)
    case ATTR_PONTOS_TURISTICOS: return (double)carta->numPontosTuristicos; // Converte para double
    case ATTR_DENSIDADE: return carta->densidadePopulacional;
    case ATTR_PIB_PER_CAPITA: return carta->pibPerCapita;
    default: return 0.0; // Valor padrão para ID de atributo desconhecido.
    }
}

/**
 * @brief Permite ao jogador selecionar um atributo de uma lista dinâmica.
 *
 * Exibe um menu com os atributos disponíveis para seleção. Se um atributo já foi
 * escolhido anteriormente (indicado por 'atributoJaEscolhido'), ele não é listado.
 * A função garante que o jogador faça uma escolha válida.
 *
 * @param atributoJaEscolhido O ID do atributo que NÃO deve ser listado como opção (0 se nenhum).
 * @param textoPrompt Mensagem a ser exibida ao jogador antes do menu de seleção.
 * @return int O ID do atributo escolhido pelo jogador.
 */
int selecionarAtributo(int atributoJaEscolhido, const char* textoPrompt) {
    int escolhaInput;                   // Armazena a entrada numérica do jogador.
    int idAtributoEscolhido = 0;        // ID final do atributo escolhido (0 = nenhum ainda).
    int i;                              // Contador para o loop de atributos.
    // Array para mapear a opção do menu (1, 2, 3...) ao ID real do atributo (ATTR_*).
    int opcaoMenuValida[NUM_ATRIBUTOS_TOTAL + 1];
    int contadorOpcoesExibidas;         // Quantas opções são de fato mostradas no menu dinâmico.

    printf("\n%s\n", textoPrompt);
    printf("----------------------------------------\n");

    // Loop continua até que uma escolha válida de atributo seja feita.
    while (idAtributoEscolhido == 0) {
        contadorOpcoesExibidas = 0; // Reseta para cada tentativa de exibição do menu.
        // Itera por todos os IDs de atributo possíveis (1 a NUM_ATRIBUTOS_TOTAL).
        for (i = 1; i <= NUM_ATRIBUTOS_TOTAL; i++) {
            // Se o atributo 'i' não for o 'atributoJaEscolhido', ele é uma opção válida.
            if (i != atributoJaEscolhido) {
                contadorOpcoesExibidas++; // Incrementa o número da opção de menu.
                printf("%d. %s\n", contadorOpcoesExibidas, obterNomeAtributo(i));
                // Mapeia a opção exibida (ex: 1) para o ID real do atributo (ex: ATTR_AREA).
                opcaoMenuValida[contadorOpcoesExibidas] = i;
            }
        }
        printf("Escolha uma opcao (1-%d): ", contadorOpcoesExibidas);

        // Tenta ler a entrada do jogador.
        if (scanf("%d", &escolhaInput) != 1) { // Verifica se a entrada foi um número.
            printf("Entrada invalida. Por favor, digite um numero.\n");
            limparBufferEntrada(); // Limpa a entrada inválida.
            continue; // Volta para o início do loop while.
        }
        limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.

        // Verifica se a escolha está dentro do range de opções exibidas.
        if (escolhaInput >= 1 && escolhaInput <= contadorOpcoesExibidas) {
            idAtributoEscolhido = opcaoMenuValida[escolhaInput]; // Obtém o ID real do atributo.
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
    printf("Voce escolheu: %s\n", obterNomeAtributo(idAtributoEscolhido));
    return idAtributoEscolhido; // Retorna o ID do atributo selecionado.
}


// --- Função Principal (main) ---
// Ponto de entrada e fluxo principal do jogo Super Trunfo.
int main() {
    // Inicialização de duas cartas pré-definidas para o jogo.
    // Os valores são exemplos e podem ser ajustados.
    CartaSuperTrunfo carta1 = { "Brasil", 210000000, 8510000.0, 1.6, 7500 };
    CartaSuperTrunfo carta2 = { "Japao",  125000000, 377975.0,  5.0, 9000 };

    // Calcula os atributos derivados (densidade, pib per capita) para ambas as cartas.
    calcularAtributosDerivados(&carta1);
    calcularAtributosDerivados(&carta2);

    // system("cls"); // Comando para limpar tela no Windows.
    // system("clear"); // Comando para limpar tela no Linux/macOS.
    // (Descomente um dos comandos acima se desejar limpar a tela).

    printf("===== SUPER TRUNFO - DESAFIO AVANCADO =====\n");
    printf("Cartas na disputa:\n");
    printf("Carta 1: %s\n", carta1.nomePais);
    printf("Carta 2: %s\n", carta2.nomePais);
    printf("----------------------------------------\n");

    // Jogador escolhe o primeiro atributo para a comparação.
    // O '0' como segundo argumento significa que nenhum atributo foi escolhido ainda.
    int idAtributo1 = selecionarAtributo(0, "Jogador, escolha o PRIMEIRO atributo para comparar:");

    // Jogador escolhe o segundo atributo.
    // 'idAtributo1' é passado para garantir que o segundo atributo seja diferente do primeiro.
    int idAtributo2 = selecionarAtributo(idAtributo1, "Agora, escolha o SEGUNDO atributo (diferente do primeiro):");

    // Obtém os valores dos atributos escolhidos para ambas as cartas.
    double valorCarta1Atr1 = obterValorAtributo(&carta1, idAtributo1);
    double valorCarta1Atr2 = obterValorAtributo(&carta1, idAtributo2);
    double valorCarta2Atr1 = obterValorAtributo(&carta2, idAtributo1);
    double valorCarta2Atr2 = obterValorAtributo(&carta2, idAtributo2);

    // Calcula a soma dos valores dos dois atributos escolhidos para cada carta.
    // A carta com a maior soma destes dois atributos vence a rodada.
    double somaCarta1 = valorCarta1Atr1 + valorCarta1Atr2;
    double somaCarta2 = valorCarta2Atr1 + valorCarta2Atr2;

    // Exibição detalhada dos resultados da rodada.
    printf("\n--- RESULTADO DA RODADA ---\n");
    printf("Cartas: %s vs %s\n", carta1.nomePais, carta2.nomePais);
    printf("Atributos Comparados:\n");
    printf("1. %s\n", obterNomeAtributo(idAtributo1));
    printf("2. %s\n", obterNomeAtributo(idAtributo2));
    printf("----------------------------------------\n");

    // Mostra os valores e a soma para a Carta 1.
    printf("Valores para %s:\n", carta1.nomePais);
    printf("  - %s: %.2f\n", obterNomeAtributo(idAtributo1), valorCarta1Atr1);
    printf("  - %s: %.2f\n", obterNomeAtributo(idAtributo2), valorCarta1Atr2);
    printf("  - Soma dos atributos para %s: %.2f\n", carta1.nomePais, somaCarta1);
    printf("----------------------------------------\n");

    // Mostra os valores e a soma para a Carta 2.
    printf("Valores para %s:\n", carta2.nomePais);
    printf("  - %s: %.2f\n", obterNomeAtributo(idAtributo1), valorCarta2Atr1);
    printf("  - %s: %.2f\n", obterNomeAtributo(idAtributo2), valorCarta2Atr2);
    printf("  - Soma dos atributos para %s: %.2f\n", carta2.nomePais, somaCarta2);
    printf("----------------------------------------\n");

    // Determina o resultado da rodada (quem venceu ou se houve empate).
    char* mensagemFinal;
    // Verifica se houve empate na soma dos atributos.
    if (somaCarta1 == somaCarta2) {
        mensagemFinal = "Empate!";
    }
    else {
        // Se não houve empate, determina o país vencedor usando um operador ternário.
        char* paisVencedor = (somaCarta1 > somaCarta2) ? carta1.nomePais : carta2.nomePais;

        // Cria a mensagem de vitória.
        // Usar um buffer estático é simples para este exemplo, mas para funções mais complexas
        // que retornam strings, alocação dinâmica ou passagem de buffer como parâmetro são mais seguras.
        static char bufferMensagem[100];
        // snprintf é mais seguro que sprintf pois evita estouro de buffer.
        snprintf(bufferMensagem, sizeof(bufferMensagem), "%s venceu a rodada!", paisVencedor);
        mensagemFinal = bufferMensagem;
    }

    // Exibe a mensagem final da rodada.
    printf("Resultado Final: %s\n", mensagemFinal);
    printf("----------------------------------------\n");

    // Pausa para o usuário ler o resultado antes do programa encerrar.
    printf("\nPressione Enter para sair...");
    limparBufferEntrada(); // Garante que qualquer Enter anterior seja consumido.
    // Se o buffer já estiver limpo, getchar() aguardará nova entrada.
    getchar(); // Aguarda uma nova entrada (Enter) para fechar.

    return 0; // Indica que o programa terminou com sucesso.
}