#pragma once
#include <vector>
#include "zorro.h"

void RecalcularPesos(double PesoMaximo, std::vector<double>* Pesos, std::vector<string>* Ativos);
void SelecionarAtivos(std::vector<string>* Ativos);
int Quantidade(std::vector<double>* Pesos, int AtivoIndex);
void CalcRetornos(std::vector<double*>*);
void CalcRetornos(std::vector<double*>* Retornos, std::vector<string>* Ativos);