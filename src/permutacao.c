#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "permutacao.h"

#define ZERO 0
#define UM 1
#define DOIS 2


// Variáveis globais.

bool foiInicializado = false;
int capacidadeCaminhao = ZERO;
struct Cidade *cidades = NULL;
int **distanciaCidades = NULL;


// Protótipos de funções.

void fazCombinacoes(int *arr, int *data, int inicio, int fim, int idx, int r, int *melhorDist, int *pMelhorDistValor);

void fazPermutacoes(int *data, int n, int tamanho, int *melhorDist, int *pMelhorDistValor);

void troca(int *a, int *b);

void checaPermutacao(const int *permutacao, int tamanho, int *melhorDist, int *pMelhorDistValor);

void liberaMemoria(int numCidades);


/*
 * Inicializa variáveis globais para serem usadas em outras funções.
 *
 * @param    arrayCidades       ponteiro para array que contém as estruturas cidades.
 * @param    numCidades         a quantidade de cidades.
 * @param    matrizDistancias   ponteiro para a matriz de distâncias entre as cidades.
 * @param    cargaCaminhao      a capacidade do caminhão.
 */
void inicializa(struct Cidade *arrayCidades, int numCidades, int **matrizDistancias, int cargaCaminhao)
{
    distanciaCidades = (int **) malloc(numCidades * sizeof(int *));

    for (int i = ZERO; i < numCidades; i++)
    {
        distanciaCidades[i] = (int *) malloc(numCidades * sizeof(int));

        for (int j = ZERO; j < numCidades; j++)
        {
            distanciaCidades[i][j] = matrizDistancias[i][j];
        }
    }

    cidades = (struct Cidade *) malloc(numCidades * sizeof(struct Cidade));

    for (int i = ZERO; i < numCidades; i++)
    {
        cidades[i] = arrayCidades[i];
    }

    capacidadeCaminhao = cargaCaminhao;
    foiInicializado = true;
}


/*
 *  Calcula todas as possibilidades de permutações de combinações de um dado array de inteiros para todas as variações de quantidade
 *  de itens. Possui como objetivo encontrar a melhor rota.
 *
 *  @param   itens          ponteiro para o array que possui os itens.
 *  @param   rotaFinal      ponteiro para o array que armazenará a rota final (a melhor rota encontrada).
 *  @param   qtdItens       número de possibilidades para começar (o número de itens).
 */
void encontraMelhorRota(int *itens, int *rotaFinal, int *pIndexArr, int qtdItens)
{
    // Retorna caso as variáveis globais não foram inicializadas.
    if (!foiInicializado)
    {
        return;
    }

    int combinacaoAtual[qtdItens];

    int melhorDistancia = ZERO;
    int *pMelhorDistValor = &melhorDistancia;

    int melhorDist[qtdItens];
    int qtdDigitosAtual = qtdItens;

    while (qtdDigitosAtual)
    {
        fazCombinacoes(itens, combinacaoAtual, ZERO, qtdItens - UM, ZERO, qtdDigitosAtual, melhorDist, pMelhorDistValor);

        // Se encontrar cidade que passe nas condições.
        if (melhorDistancia != ZERO)
        {
            // Seta para zero novamente para checar se outra permutação passa nas condições.
            melhorDistancia = ZERO;

            // Adiciona zero no início da permutação.
            rotaFinal[(*pIndexArr)++] = ZERO;

            // Adiciona cidades encontrados no itens final.
            for (int i = ZERO; i < qtdDigitosAtual; i++)
            {
                rotaFinal[(*pIndexArr)++] = melhorDist[i];
                cidades[melhorDist[i]].foiVisitada = true;
            }

            // Adiciona zero no final da permutação.
            rotaFinal[(*pIndexArr)++] = ZERO;

            // Checa a mesmo digito novamente para ver se há mais permutações com o mesmo número de dígitos.
            continue;
        }

        qtdDigitosAtual--;
    }

    // Liberando a memória sendo usada pelas variáveis globais.
    liberaMemoria(qtdItens);
}


/*
 * Calcula todas as possibilidades de combinações de um dado array de inteiros com uma quantidade de itens específica.
 * Utiliza recursão para tal cálculo.
 *
 * @param    arr                 ponteiro para o array que possui os itens.
 * @param    data                ponteiro para o array que irá armazenar uma possibilidade por vez.
 * @param    inicio              index do início do array que armazena uma possibilidade por vez.
 * @param    fim                 index do fim do array que armazena uma possibilidade por vez.
 * @param    idx                 index atual do array que armazena uma possibilidade por vez.
 * @param    r                   número de itens sendo escolhido.
 * @param    melhorDist          ponteiro para array que contém melhor permutação encontrada até então.
 * @param    pMelhorDistValor    ponteiro para valor da distância total da melhor permutação encontrada até então.
 */
void fazCombinacoes(int *arr, int *data, int inicio, int fim, int idx, int r, int *melhorDist, int *pMelhorDistValor)
{
    // Entra nessa condição quando tiver uma combinação pronta.
    if (idx == r)
    {
        // Criando outro array, pois em C não existe "pass by value" quando usando arrays. Em outras palavras, criando array para ser
        // modificado.
        int novoData[r];

        for (int i = ZERO; i < r; i++)
        {
            novoData[i] = data[i];
        }

        // Realizando permutações a partir da combinação encontrada.
        fazPermutacoes(novoData, r, r, melhorDist, pMelhorDistValor);

        return;
    }

    for (int i = inicio; i <= fim && fim - i + UM >= r - idx; i++)
    {
        data[idx] = arr[i];
        fazCombinacoes(arr, data, i + UM, fim, idx + UM, r, melhorDist, pMelhorDistValor);
    }
}


/*
 * Usa a permutação de Heap para gerar todas as permutações de forma recursiva.
 *
 * @param    data                array para ser permutado.
 * @param    n                   tamanho do array.
 * @param    tamanho             tamanho do array.
 * @param    melhorDist          ponteiro para array que contém melhor permutação encontrada até então.
 * @param    pMelhorDistValor    ponteiro para valor da distância total da melhor permutação encontrada até então.
 */
void fazPermutacoes(int *data, int n, int tamanho, int *melhorDist, int *pMelhorDistValor)
{
    // Entra nessa condição se tiver uma permutação pronta.
    if (n == UM)
    {
        // Checa se a permutação feita é melhor que as anteriores.
        checaPermutacao(data, tamanho, melhorDist, pMelhorDistValor);

        return;
    }

    for (int i = ZERO; i < n; i++)
    {
        fazPermutacoes(data, n - UM, tamanho, melhorDist, pMelhorDistValor);

        if (n % DOIS == UM)
        {
            troca(data, data + (n - UM));
        }
        else
        {
            troca(data + i, data + (n - UM));
        }
    }
}


/*
 * Troca valores dos elementos dados na memória.
 *
 * @param    a    primeiro valor.
 * @param    b    segundo valor.
 */
void troca(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


/*
 * Checa se a permutação atual dada é melhor que as permutações anteriores.
 *
 * @param    permutacao           ponteiro para o array com a permutação.
 * @param    tamanho              tamanho do array com a permutação.
 * @param    melhorDist           ponteiro para array que contém melhor permutação encontrada até então.
 * @param    pMelhorDistValor     ponteiro para valor da distância total da melhor permutação encontrada até então.
 */
void checaPermutacao(const int *permutacao, int tamanho, int *melhorDist, int *pMelhorDistValor)
{
    unsigned int demandaTotal = ZERO;

    for (int i = ZERO; i < tamanho; i++)
    {
        // Associando com index em cidades.
        int cidadeAtual = permutacao[i];

        // Checa se tem alguma cidade que já foi visitada na permutação, caso haja retorna, ignorando a permutação.
        if (cidades[cidadeAtual].foiVisitada)
        {
            return;
        }

        demandaTotal += cidades[cidadeAtual].demanda;
    }

    // Essa variável é usada para determinar se dada a demanda, temos que procurar por uma permutação igual ou maior.
    // Dessa forma conseguimos suprir a quantidade de caminhões necessários.
    // Isso conserta o erro de termos uma rota mais optimizada, porém a mesma iria ultrapassar a quantidade de caminhões que possuímos.
    bool otimizaRota = (demandaTotal <= capacidadeCaminhao);

    if ((demandaTotal % capacidadeCaminhao) == ZERO)
    {
        otimizaRota = (demandaTotal == capacidadeCaminhao);
    }

    if (otimizaRota)
    {
        int distanciaTotal = ZERO;

        // Calcula a distância do depósito até a primeira cidade.
        distanciaTotal = distanciaCidades[ZERO][permutacao[ZERO]];

        // Calcula a distância entre as cidades na combinação.
        for (int i = ZERO; i < tamanho - UM; i++)
        {
            distanciaTotal += distanciaCidades[permutacao[i]][permutacao[i + UM]];
        }

        // Calcula a distância da última cidade até o depósito.
        distanciaTotal += distanciaCidades[ZERO][permutacao[tamanho - UM]];

        // Checa se a permutação é melhor que as anteriores.
        // Também entra na condição se for a primeira permutação que passou nas condições anteriores.
        if (distanciaTotal < *pMelhorDistValor || *pMelhorDistValor == ZERO)
        {
            *pMelhorDistValor = distanciaTotal;

            for (int i = ZERO; i < tamanho; i++)
            {
                melhorDist[i] = permutacao[i];
            }
        }
    }
}


/*
 * Libera memória das estruturas globais alocadas dinamicamente.
 *
 * @param    numCidades    número de cidades.
 */
void liberaMemoria(int numCidades)
{
    // Retorna caso as variáveis globais não foram inicializadas.
    if (!foiInicializado)
    {
        return;
    }

    for (int i = ZERO; i < numCidades; i++)
    {
        free(distanciaCidades[i]);
    }

    free(distanciaCidades);
    free(cidades);
}