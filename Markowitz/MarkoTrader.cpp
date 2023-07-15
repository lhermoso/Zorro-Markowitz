#include "pch.h"
#include <vector>
#include "zorro.h"
#include "Markowitz.h"

#define DAYS	252 	

using std::vector;






DLLFUNC void run()
{
	BarPeriod = 1440;
	LookBack = DAYS;
	NumYears = 10;
	Verbose = 0;
	Capital = 100000;
	set(PRELOAD + LOG);
	assetList("history\\bovespa_yahoo.csv", 0);
	static vector<double> Pesos;
	static vector<string> Ativos;



	bool rebalancear = tdm(0) == 1 && month(0) % 3 == 0;

	//INITRUN -> PRIMEIRA VEZ QUE O ALGO RODA SEM DADOS CARREGADOS
	//FIRSTRUN -> PRIMEIRA VEZ QUE O ALGO RODA COM DADOS CARREGADOS




	if (rebalancear || is(FIRSTRUN))
	{
		RecalcularPesos(0.5, &Pesos, &Ativos);


	}
	string _Asset;
	while (_Asset = (loop(Assets)))
	{
		if (is(INITRUN))
			assetHistory(_Asset, FROM_YAHOO);
		asset(_Asset);

		if (rebalancear && NumOpenLong > 0)
		{

			bool foraDaLista = true;

			if (std::find(Ativos.begin(), Ativos.end(), _Asset) != Ativos.end())
			{
				foraDaLista = false;

			}

			if (foraDaLista)
				exitLong();
		}


	}

	int ativoIndex = 0;
	if (rebalancear)
	{
		for (auto& ativo : Ativos) {

			asset(ativo);


			int quantidade = Quantidade(&Pesos, ativoIndex);


			if (NumOpenLong > 0)
			{

				int quantidades_diff = LotsPool - quantidade;
				if (quantidades_diff > 0)
				{
					exitLong(0, 0, quantidades_diff);
				}
				else if (quantidades_diff < 0)
				{
					enterLong(-quantidades_diff);

				}

			}
			else if (quantidade > 0)
			{

				enterLong(quantidade);
			}



			++ativoIndex;


		}
	}







}