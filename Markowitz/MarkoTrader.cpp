#include "pch.h"
#include <vector>
#include "zorro.h"
#include "Markowitz.h"

#define DAYS   252
#define OPENTRADE true

using std::vector;



DLLFUNC void run()
{
	BarPeriod = 1440;
	LookBack = DAYS;
	NumYears = 10;
	StartDate = NOW;
	//StartMarket = 1300;
	//EndMarket = 2100;
	Verbose = 0;
	Capital = slider(1, 100000, 0, 1000000, "Capital", "Capital Total Pra Alocar");
	set(PRELOAD + LOG + PLOTNOW);
	assetList("history\\bovespa_yahoo.csv", 0);
	static vector<double> Pesos;
	static vector<string> Ativos;



	bool rebalancear = (tdm(0) == 5 && (month(0) == 9 || month(0) == 5 || month(0) == 1 || month(0) % 3 == 0 ) || (Live && OPENTRADE));



	if (rebalancear)
	{
		RecalcularPesos(0.1, &Pesos, &Ativos);


	}
	string _Asset;
	while ((_Asset = (loop(Assets))))
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
				if (!is(LOOKBACK))
				{
					enterLong(quantidade);
				}
			}
			++ativoIndex;

		}
	}

	if (is(EXITRUN)) {
		int ativoIndex = 0;
		double total = 0;
		for (auto& ativo : Ativos) {
			asset(ativo);
			int quantidade = Quantidade(&Pesos, ativoIndex);
			if (quantidade > 0)
			{
				int lot_size = LotAmount;
				total += quantidade * lot_size * priceClose(0);
				printf("%s - %i acoes a %.5f\n", ativo, quantidade * lot_size, priceClose(0));
				if (OPENTRADE)
				{
					enterLong(quantidade);
				}
				char res[250];
				sprintf(res, "% s - % i acoes a % .2f\n", ativo, quantidade * lot_size, priceClose(0));
				file_append("Data\\markowitz_trades.txt", res);
			}
			++ativoIndex;
		}
		printf("Total Alocado: %.2f\n", total);


	}





}