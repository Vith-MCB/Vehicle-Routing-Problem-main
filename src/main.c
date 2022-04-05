#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "permutacao.h"

// Varia conforme o sistema operacional.
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
#define PATH_ARQUIVO "..\\arquivos\\"
#else
#define PATH_ARQUIVO "../arquivos/" // Edite caso você estiver em outro plataforma que não seja Windows.
#endif

#define ZERO 0
#define UM 1
#define DOIS 2
#define TRES 3
#define ERRO (-1)
#define ERRO_ABRIR_ARQUIVO "\nErro ao tentar abrir o arquivo.\n"
#define RECEBE_NOME_ARQUIVO "Digite o nome do arquivo: \nOBS: O arquivo deve estar na pasta 'arquivos'.\n"
#define QTD_CAMINHOES "\nQuantidade de caminhoes: %d.\n"
#define MELHOR_ROTA "\nMELHOR ROTA ENCONTRADA: \n"
#define TEMPO_TOTAL "\n\nTempo total = %0.4lf segundos.\n"


// Protótipos de funções.

double inicializaClock(void);

double calculaTempo(double tempoInicial);

void cleanUp(struct Cidade *cidades, int **distanciaCidades, int qtdCidades);


/*
 * A função principal. Tem como função a leitura de informações de um arquivo e manipulação dos dados.
 *
 * @return          0 se executado com sucesso, -1 se ocorrer algum erro.
 */
int main(void)
{

    /// --------------------------------------- CRIANDO CAMINHO PARA ARQUIVO -------------------------------------------

    // Inicializando ponteiro do arquivo.
    FILE *pArquivo = NULL;

    // Inicializando variável para o nome do arquivo.
    char nomeArquivo[CHAR_MAX];

    // Lendo nome do arquivo do usuário.
    printf(RECEBE_NOME_ARQUIVO);
    scanf("%s", nomeArquivo);

    // Calculando tamanho do caminho para o arquivo. Adicionando um para o null terminator em Strings.
    char caminhoArquivo[strlen(PATH_ARQUIVO) + strlen(nomeArquivo) + UM];

    // Juntando nome do arquivo com caminho para o arquivo.
    strcpy(caminhoArquivo, PATH_ARQUIVO);
    strcat(caminhoArquivo, nomeArquivo);

    /// ------------------------------------ FIM CRIANDO CAMINHO PARA ARQUIVO ------------------------------------------


    // Inicializando o clock.
    double inicioClock = inicializaClock();


    /// ------------------------------------------- OPERAÇÕES NO ARQUIVO -----------------------------------------------

    // Abrindo arquivo.
    pArquivo = fopen(caminhoArquivo, "r");

    // Lidando com caso que o arquivo não é encontrado.
    if (!pArquivo)
    {
        printf(ERRO_ABRIR_ARQUIVO);
        // Retornando caso não encontre o arquivo.
        return ERRO;
    }

    // Inicializando variáveis.
    int qtdCidades = ZERO;
    int cargaCaminhao = ZERO;
    int somatorioDemandas = ZERO;

    // Lendo a quantidade de cidades (N).
    fscanf(pArquivo, "%d", &qtdCidades);

    // Lendo a capacidade do caminhão (Qv).
    fscanf(pArquivo, "%d", &cargaCaminhao);

    // Definindo array da estrutura Cidade para armazenar as cidades.
    struct Cidade *cidades = (struct Cidade *) malloc(qtdCidades * sizeof(struct Cidade));

    // Alocando matriz para armazenar as distâncias entre as cidades e depósito.
    int **distanciaCidades = (int **) malloc(qtdCidades * sizeof(int *));

    for (int i = ZERO; i < qtdCidades; i++)
    {
        distanciaCidades[i] = (int *) malloc(qtdCidades * sizeof(int));
    }

    // Array com os elementos para fazer permutações. Tirando o depósito.
    int itens[qtdCidades - UM];

    // Lendo as demandas de cada cidade e criando estrutura para cada cidade.
    for (int i = ZERO, j = ZERO; i < qtdCidades; i++)
    {
        struct Cidade cidade;
        fscanf(pArquivo, "%d", &cidade.demanda);
        cidade.foiVisitada = false;

        // Adicionando cidade no array de cidades.
        cidades[i] = cidade;

        // Colocando as cidades com exceção do depósito em um array para realizar permutações.
        if (i != ZERO)
        {
            itens[j++] = i;
        }

        somatorioDemandas += (int) cidade.demanda;
    }

    // Calculando a quantidade de caminhões.
    unsigned int qtdCaminhoes = (unsigned int) ceil((double) somatorioDemandas / cargaCaminhao);
    printf(QTD_CAMINHOES, qtdCaminhoes);

    // Colocando valores do arquivo em uma matriz simétrica.
    for (int i = ZERO; i < qtdCidades; i++)
    {
        for (int j = ZERO; j < qtdCidades; j++)
        {
            if (i == j)
            {
                distanciaCidades[i][j] = ZERO;
            }
            else
            {
                int ii = ZERO;
                int jj = ZERO;

                fscanf(pArquivo, "%d", &ii);
                fscanf(pArquivo, "%d", &jj);
                fscanf(pArquivo, "%d", &distanciaCidades[ii][jj]);
            }
        }
    }

    // Fim das operações no arquivo.
    fclose(pArquivo); // Fechando o arquivo.
    pArquivo = NULL; // Setando ponteiro como nulo.

    /// -------------------------------------- FIM DE OPERAÇÕES NO ARQUIVO ---------------------------------------------


    /// ----------------------------------------- PROCESSAMENTO DOS DADOS ----------------------------------------------

    // Cria array para rota final baseado na maior capacidade que ele possa ter.
    int rotaFinal[(qtdCidades - UM) * TRES];

    // Definindo variável e ponteiro para ela para ser modificada dentro de uma função.
    int pIdxValor = ZERO;
    int *pIdxArr = &pIdxValor;

    // Inicializa variáveis globais no outro arquivo.
    inicializa(cidades, qtdCidades, distanciaCidades, cargaCaminhao);

    // Chama função que faz todas as permutações a partir de combinações.
    encontraMelhorRota(itens, rotaFinal, pIdxArr, qtdCidades - UM);

    // Printando melhor rota encontrada.
    printf(MELHOR_ROTA);
    for (int i = ZERO; i < pIdxValor; i++)
    {
        printf("%d ", rotaFinal[i]);
    }

    // Libera espaço na memória alocada por estruturas.
    cleanUp(cidades, distanciaCidades, qtdCidades);

    /// ------------------------------------- FIM DO PROCESSAMENTO DOS DADOS -------------------------------------------


    // Calculando tempo total de execução do programa.
    double tempoTotal = calculaTempo(inicioClock);

    printf(TEMPO_TOTAL, tempoTotal);


    return ZERO;
}


/*
 * Inicia o clock.
 * @return      valor de quando a função "clock()" é chamada.
 */
double inicializaClock(void)
{
    return (double) clock();
}


/*
 * Finaliza o clock e calcula o tempo total.
 *
 * @param    tempoInicial    valor da primeira vez que a função "clock()" foi chamada.
 * @return                   valor calculado referente ao tempo entre o tempo inicial e o tempo final.
 */
double calculaTempo(double tempoInicial)
{
    return (double) (clock() - tempoInicial) / CLOCKS_PER_SEC;
}


/*
 * Libera espaço na memória criado para estruturas.
 *
 * @param    cidades             ponteiro para o array que contém as estruturas Cidade.
 * @param    distanciaCidades    ponteiro para matriz que contém as distâncias entre as cidades.
 * @param    qtdCidades          quantidade de cidades.
 */
void cleanUp(struct Cidade *cidades, int **distanciaCidades, int qtdCidades)
{
    for (int i = ZERO; i < qtdCidades; i++)
    {
        free(distanciaCidades[i]);
    }

    free(distanciaCidades);
    free(cidades);
}