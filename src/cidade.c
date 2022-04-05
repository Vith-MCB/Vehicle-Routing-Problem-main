#include "cidade.h"

unsigned int getDemanda(struct Cidade cidade)
{
    return cidade.demanda;
}

bool getFoiVisitada(struct Cidade cidade)
{
    return cidade.foiVisitada;
}

void setDemanda(struct Cidade cidade, unsigned int novaDemanda)
{
    cidade.demanda = novaDemanda;
}

void setFoiVisitada(struct Cidade cidade, bool foiVisitada)
{
    cidade.foiVisitada = foiVisitada;
}