#include "pch.h"
#include <vector>
#include "zorro.h"
#include "Markowitz.h"

void RecalcularPesos(double PesoMaximo, std::vector<double>* Pesos, std::vector<string>* Ativos)
{
	std::vector<double*> Retornos;
	std::vector<double> MediaRetornos;
	std::vector<double> Covariancias;
	SelecionarAtivos(Ativos);
	CalcRetornos(&Retornos, Ativos);
	Pesos->clear();

	int numAtivos = Ativos->size();
	int i, j;

	for (i = 0; i < numAtivos; ++i)
	{
		Pesos->push_back(0);
		MediaRetornos.push_back(Moment(Retornos.at(i), LookBack, 1));
		for (j = 0; j < numAtivos; ++j)
		{
			Covariancias.push_back(Covariance(Retornos.at(i), Retornos.at(j), LookBack));
		}

	}
	if (numAtivos)
	{
		double MelhorVariancia = markowitz(&(Covariancias)[0], &(MediaRetornos)[0], numAtivos, PesoMaximo);
		markowitzReturn(&(*(Pesos))[0], MelhorVariancia);
	}


}

void SelecionarAtivos(std::vector<string>* Ativos)
{

	std::vector<double*> Retornos;
	std::vector<double> retornos;
	std::vector<double> variancia;
	CalcRetornos(&Retornos);
	Ativos->clear();
	int numAtivos = Retornos.size();
	int i;
	for (i = 0; i < numAtivos; ++i)
	{

		retornos.push_back(Moment(Retornos.at(i), LookBack , 1));
		variancia.push_back(Moment(Retornos.at(i), LookBack,  2));

	}

	double retorno_medio = Moment(&(retornos)[0], numAtivos, 1);
	double variancia_media = Moment(&(variancia)[0], numAtivos, 1);
	double std_retornos = Moment(&(retornos)[0], numAtivos  , 2);

	for (i = 0; i < numAtivos; ++i)
	{
		if (retornos.at(i) > retorno_medio + std_retornos *2  && variancia.at(i) < variancia_media)
		{
			Ativos->push_back(Assets[i]);

		}
	}

}

int Quantidade(std::vector<double>* Pesos, int AtivoIndex)
{
	double capital = Capital * sqrt(1 + (ProfitClosed / Capital)) * OptimalFLong;
	return round(Pesos->at(AtivoIndex) * capital / priceClose(0) / LotAmount);
}



void CalcRetornos(std::vector<double*>* Retornos)
{
	int ativoIndex = 0;
	int dayIndex = 0;
	Retornos->clear();

	while (asset(loop(Assets)))
	{
		Retornos->push_back(new double[LookBack]);
		for (dayIndex = 0; dayIndex < LookBack; ++dayIndex)
		{
			Retornos->at(ativoIndex)[dayIndex] = (priceClose(dayIndex) - priceClose(dayIndex + 1)) / priceClose(dayIndex + 1);

		}
		++ativoIndex;

	}

}


void CalcRetornos(std::vector<double*>* Retornos, std::vector<string>* Ativos)
{
	int ativoIndex = 0;
	int dayIndex = 0;
	Retornos->clear();

	for (auto& ativo : *Ativos)
	{
		asset(ativo);
		Retornos->push_back(new double[LookBack]);
		for (dayIndex = 0; dayIndex < LookBack; ++dayIndex)
		{
			Retornos->at(ativoIndex)[dayIndex] = (priceClose(dayIndex) - priceClose(dayIndex + 1)) / priceClose(dayIndex + 1);

		}
		++ativoIndex;



	}

}