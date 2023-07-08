#include "pch.h"
#include <vector>
#include <algorithm>   
#include "zorro.h"
#include "ativos.h"



using std::vector;
#define NN		88	 // max number of assets

double annual(var daily)
{
	return pow(daily + 1., 252) - 1.;
}

vector<string> melhoresAtivos()
{

	vector<string> ativos;


	string	Names[NN];
	double* Returns[NN];
	double	Correlations[NN][NN];
	
	int N = 0;
	while (Names[N] = loop(
		Assets,0))
	{
		asset(Names[N]);
		Returns[N] = series((priceClose(0) - priceClose(1)) / priceClose(1));
		if (N++ >= NN) break;
	}
	//if (is(EXITRUN)) {
		int i, j;
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				Correlations[i][j] = Correlation(Returns[i], Returns[j], LookBack);

		for (i = 0; i < N; i++)
		{
			double variance = 252 * 100 * Moment(Returns[i], LookBack, 2);
			double mean_return = 100 * annual(Moment(Returns[i], LookBack, 1));

			if (variance < 15 )
			{
				ativos.push_back(Assets[i]);
			}
		}

		return ativos;
		/*i = 0;
		j = 0;
		const int total_ativos = ativos.size();

		vector<int> ativos_remove;
		
		for (vector<int>::iterator it = ativos.begin(); it != ativos.end(); ++it)
			for (vector<int>::iterator ij = ativos.begin(); ij != ativos.end(); ++ij)
			{
				if (*it == *ij)
					continue;
				if (Correlations[*it][*ij] > 0.5)
				{
					ativos_remove.push_back(*ij);
				}
			}

		std::sort(begin(ativos_remove), end(ativos_remove));
		ativos.erase(remove_if(begin(ativos), end(ativos),
			[&](auto x) {return binary_search(begin(ativos_remove), end(ativos_remove), x); }), end(ativos));


		vector<string> ativos_retorno;
		
		i = 0;
		for (vector<int>::iterator it = ativos.begin(); it != ativos.end(); ++it)
		{

			ativos_retorno.push_back(Assets[*it]);
			double variance = 252 * 100 * Moment(Returns[*it], LookBack, 2);
			double mean_return = 100 * annual(Moment(Returns[*it], LookBack, 1));

			printf("\n%i - %s: Mean %.2f%%  Variance %.2f%%",
				i + 1, Names[*it], mean_return, variance);
			
		}
		return ativos_retorno;*/
		
		//	quit("");
	//}
}