#define _CRT_SECURE_NO_WARNINGS // Necess�rio para o Visual Studio desabilitar avisos de scanf, etc.
#include <stdio.h>              // Para fun��es de entrada/sa�da padr�o (printf, scanf, getchar)
#include <string.h>             // Para fun��es de manipula��o de strings (strcpy, strcmp, etc. - aqui usamos para sprintf)
#include <stdlib.h>             // Para fun��es utilit�rias gerais (system - n�o usado ativamente para manter portabilidade)

// --- Constantes para os IDs dos Atributos ---
// Estes defines facilitam a leitura e manuten��o do c�digo ao referenciar atributos.
#define ATTR_POPULACAO 1
#define ATTR_AREA 2
#define ATTR_PIB 3
#define ATTR_PONTOS_TURISTICOS 4
#define ATTR_DENSIDADE 5
#define ATTR_PIB_PER_CAPITA 6
#define NUM_ATRIBUTOS_TOTAL 6 // Total de atributos dispon�veis para sele��o

// --- Estrutura da Carta Super Trunfo ---
// Define os campos de dados para cada carta do jogo.
typedef struct {
    char nomePais[50];              // Nome do pa�s representado pela carta
    unsigned long int populacao;    // Popula��o do pa�s (unsigned long int para n�meros grandes)
    double area;                    // �rea do pa�s em km�
    double pib;                     // Produto Interno Bruto (ex: em trilh�es de USD)
    int numPontosTuristicos;        // N�mero de pontos tur�sticos relevantes
    double densidadePopulacional;   // Calculado: populacao / area (hab/km�)
    double pibPerCapita;            // Calculado: pib / populacao (USD)
} CartaSuperTrunfo;

// --- Prot�tipos das Fun��es ---
// Declara��es antecipadas das fun��es utilizadas no programa.
void limparBufferEntrada();
void calcularAtributosDerivados(CartaSuperTrunfo* carta);
const char* obterNomeAtributo(int idAtributo);
double obterValorAtributo(const CartaSuperTrunfo* carta, int idAtributo);
int selecionarAtributo(int atributoJaEscolhido, const char* textoPrompt);

/**
 * @brief Limpa o buffer de entrada do teclado.
 *
 * Consome caracteres restantes no buffer de entrada at� encontrar uma nova linha ('\n')
 * ou o final do arquivo (EOF). Essencial ap�s usar scanf para ler n�meros, antes
 * de ler strings com fgets ou para evitar que entradas residuais afetem leituras futuras.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Calcula os atributos derivados de uma carta (Densidade Populacional e PIB per Capita).
 *
 * @param carta Ponteiro para a struct CartaSuperTrunfo cujos atributos derivados ser�o calculados.
 * Os campos 'densidadePopulacional' e 'pibPerCapita' da struct ser�o atualizados.
 */
void calcularAtributosDerivados(CartaSuperTrunfo* carta) {
    // Calcula a densidade populacional, evitando divis�o por zero se a �rea for 0.
    if (carta->area > 0) {
        carta->densidadePopulacional = (double)carta->populacao / carta->area;
    }
    else {
        // Se a �rea for 0, a densidade � considerada 0 (ou poderia ser um valor indicando indefini��o).
        // Para a regra de "menor densidade � melhor", 0 � o melhor valor poss�vel.
        carta->densidadePopulacional = 0.0;
    }

    // Calcula o PIB per capita, evitando divis�o por zero se a popula��o for 0.
    if (carta->populacao > 0) {
        // Exemplo: se o PIB est� em trilh�es (10^12) na struct, ajusta para a unidade correta.
        carta->pibPerCapita = (carta->pib * 1e12) / (double)carta->populacao;
    }
    else {
        carta->pibPerCapita = 0.0;
    }
}

/**
 * @brief Retorna o nome de um atributo como string, dado o seu ID num�rico.
 *
 * @param idAtributo O ID num�rico do atributo (conforme definido pelas constantes ATTR_*).
 * @return const char* Uma string constante contendo o nome do atributo.
 * Retorna "Desconhecido" se o ID n�o for v�lido.
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
    default: return "Desconhecido"; // Caso o ID n�o corresponda a nenhum atributo conhecido.
    }
}

/**
 * @brief Obt�m o valor num�rico de um atributo espec�fico de uma carta.
 *
 * @param carta Ponteiro constante para a struct CartaSuperTrunfo da qual o valor ser� extra�do.
 * @param idAtributo O ID num�rico do atributo desejado.
 * @return double O valor do atributo. Retorna 0.0 se o ID do atributo for inv�lido.
 * Todos os valores s�o retornados como double para facilitar c�lculos consistentes.
 */
double obterValorAtributo(const CartaSuperTrunfo* carta, int idAtributo) {
    // Estrutura switch para retornar o valor do atributo correto da carta.
    switch (idAtributo) {
    case ATTR_POPULACAO: return (double)carta->populacao; // Converte para double
    case ATTR_AREA: return carta->area;
    case ATTR_PIB: return carta->pib; // Valor direto (ex: em trilh�es)
    case ATTR_PONTOS_TURISTICOS: return (double)carta->numPontosTuristicos; // Converte para double
    case ATTR_DENSIDADE: return carta->densidadePopulacional;
    case ATTR_PIB_PER_CAPITA: return carta->pibPerCapita;
    default: return 0.0; // Valor padr�o para ID de atributo desconhecido.
    }
}

/**
 * @brief Permite ao jogador selecionar um atributo de uma lista din�mica.
 *
 * Exibe um menu com os atributos dispon�veis para sele��o. Se um atributo j� foi
 * escolhido anteriormente (indicado por 'atributoJaEscolhido'), ele n�o � listado.
 * A fun��o garante que o jogador fa�a uma escolha v�lida.
 *
 * @param atributoJaEscolhido O ID do atributo que N�O deve ser listado como op��o (0 se nenhum).
 * @param textoPrompt Mensagem a ser exibida ao jogador antes do menu de sele��o.
 * @return int O ID do atributo escolhido pelo jogador.
 */
int selecionarAtributo(int atributoJaEscolhido, const char* textoPrompt) {
    int escolhaInput;                   // Armazena a entrada num�rica do jogador.
    int idAtributoEscolhido = 0;        // ID final do atributo escolhido (0 = nenhum ainda).
    int i;                              // Contador para o loop de atributos.
    // Array para mapear a op��o do menu (1, 2, 3...) ao ID real do atributo (ATTR_*).
    int opcaoMenuValida[NUM_ATRIBUTOS_TOTAL + 1];
    int contadorOpcoesExibidas;         // Quantas op��es s�o de fato mostradas no menu din�mico.

    printf("\n%s\n", textoPrompt);
    printf("----------------------------------------\n");

    // Loop continua at� que uma escolha v�lida de atributo seja feita.
    while (idAtributoEscolhido == 0) {
        contadorOpcoesExibidas = 0; // Reseta para cada tentativa de exibi��o do menu.
        // Itera por todos os IDs de atributo poss�veis (1 a NUM_ATRIBUTOS_TOTAL).
        for (i = 1; i <= NUM_ATRIBUTOS_TOTAL; i++) {
            // Se o atributo 'i' n�o for o 'atributoJaEscolhido', ele � uma op��o v�lida.
            if (i != atributoJaEscolhido) {
                contadorOpcoesExibidas++; // Incrementa o n�mero da op��o de menu.
                printf("%d. %s\n", contadorOpcoesExibidas, obterNomeAtributo(i));
                // Mapeia a op��o exibida (ex: 1) para o ID real do atributo (ex: ATTR_AREA).
                opcaoMenuValida[contadorOpcoesExibidas] = i;
            }
        }
        printf("Escolha uma opcao (1-%d): ", contadorOpcoesExibidas);

        // Tenta ler a entrada do jogador.
        if (scanf("%d", &escolhaInput) != 1) { // Verifica se a entrada foi um n�mero.
            printf("Entrada invalida. Por favor, digite um numero.\n");
            limparBufferEntrada(); // Limpa a entrada inv�lida.
            continue; // Volta para o in�cio do loop while.
        }
        limparBufferEntrada(); // Limpa o '\n' deixado pelo scanf.

        // Verifica se a escolha est� dentro do range de op��es exibidas.
        if (escolhaInput >= 1 && escolhaInput <= contadorOpcoesExibidas) {
            idAtributoEscolhido = opcaoMenuValida[escolhaInput]; // Obt�m o ID real do atributo.
        }
        else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
    printf("Voce escolheu: %s\n", obterNomeAtributo(idAtributoEscolhido));
    return idAtributoEscolhido; // Retorna o ID do atributo selecionado.
}


// --- Fun��o Principal (main) ---
// Ponto de entrada e fluxo principal do jogo Super Trunfo.
int main() {
    // Inicializa��o de duas cartas pr�-definidas para o jogo.
    // Os valores s�o exemplos e podem ser ajustados.
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

    // Jogador escolhe o primeiro atributo para a compara��o.
    // O '0' como segundo argumento significa que nenhum atributo foi escolhido ainda.
    int idAtributo1 = selecionarAtributo(0, "Jogador, escolha o PRIMEIRO atributo para comparar:");

    // Jogador escolhe o segundo atributo.
    // 'idAtributo1' � passado para garantir que o segundo atributo seja diferente do primeiro.
    int idAtributo2 = selecionarAtributo(idAtributo1, "Agora, escolha o SEGUNDO atributo (diferente do primeiro):");

    // Obt�m os valores dos atributos escolhidos para ambas as cartas.
    double valorCarta1Atr1 = obterValorAtributo(&carta1, idAtributo1);
    double valorCarta1Atr2 = obterValorAtributo(&carta1, idAtributo2);
    double valorCarta2Atr1 = obterValorAtributo(&carta2, idAtributo1);
    double valorCarta2Atr2 = obterValorAtributo(&carta2, idAtributo2);

    // Calcula a soma dos valores dos dois atributos escolhidos para cada carta.
    // A carta com a maior soma destes dois atributos vence a rodada.
    double somaCarta1 = valorCarta1Atr1 + valorCarta1Atr2;
    double somaCarta2 = valorCarta2Atr1 + valorCarta2Atr2;

    // Exibi��o detalhada dos resultados da rodada.
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
        // Se n�o houve empate, determina o pa�s vencedor usando um operador tern�rio.
        char* paisVencedor = (somaCarta1 > somaCarta2) ? carta1.nomePais : carta2.nomePais;

        // Cria a mensagem de vit�ria.
        // Usar um buffer est�tico � simples para este exemplo, mas para fun��es mais complexas
        // que retornam strings, aloca��o din�mica ou passagem de buffer como par�metro s�o mais seguras.
        static char bufferMensagem[100];
        // snprintf � mais seguro que sprintf pois evita estouro de buffer.
        snprintf(bufferMensagem, sizeof(bufferMensagem), "%s venceu a rodada!", paisVencedor);
        mensagemFinal = bufferMensagem;
    }

    // Exibe a mensagem final da rodada.
    printf("Resultado Final: %s\n", mensagemFinal);
    printf("----------------------------------------\n");

    // Pausa para o usu�rio ler o resultado antes do programa encerrar.
    printf("\nPressione Enter para sair...");
    limparBufferEntrada(); // Garante que qualquer Enter anterior seja consumido.
    // Se o buffer j� estiver limpo, getchar() aguardar� nova entrada.
    getchar(); // Aguarda uma nova entrada (Enter) para fechar.

    return 0; // Indica que o programa terminou com sucesso.
}