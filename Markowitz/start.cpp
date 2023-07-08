#include "pch.h"
#include "zorro.h"

//////////////////////////////////////////////////////
// Alocacao de Portifolio ////////////////////////
//////////////////////////////////////////////////////

#define LEVERAGE 1	
#define DAYS	252 	
#define NN	    80
//#define BACKTEST 
#define CARTEIRA
//#define FRONTEIRA

//////////////////////////////////////////////////////


// PVM -> PORTFOLIO VARIACANCIA MINIMA -> V=0.5 -> R=1.5
// 

/*
R$100000
xPETR4
USIM5
VALE3

*/


DLLFUNC void run()
{

	BarPeriod = 1440;
	LookBack = DAYS;
	NumYears = 2;
	Verbose = 0;
	set(PRELOAD);

	string Names[NN]{ 0 };
	vars	Returns[NN]{ 0 };
	var	Means[NN]{ 0 };
	double*	Covariances = new double[NN * NN]{ 0 };
	var	Weights[NN]{ 0 };

	var TotalCapital = slider(1, 100000, 0, 100000, "Capital", "Capital Total Pra Alocar");
	var VFactor = slider(2, 100, 0, 100, "Risco", "Fator Variancia");

	int N = 0;
	while (Names[N] = loop(
		"PETR4.SA",               // ATIVOS BAIXA CORRELACAO
		"VALE3.SA",               // RETORNO MEDIO ALTO
		"B3SA3.SA",               // VARIANCIA BAIXA
		"MGLU3.SA",
		"ITUB4.SA",
		"PETR3.SA",
		"PRIO3.SA",
		"RAIL3.SA",
		"ELET3.SA",
		"EQTL3.SA",
		"BBDC4.SA",
		"HAPV3.SA",
		"VIIA3.SA",
		"RAIZ4.SA",
		"BRFS3.SA",
		"ITSA4.SA", 0))
	{
		if (is(INITRUN))
			assetHistory(Names[N], FROM_YAHOO);
		asset(Names[N]);
		Returns[N] = series((priceClose(0) - priceClose(1)) / priceClose(1));
		if (N++ >= NN) break;
	}
#ifdef BACKTEST
	static var BestVariance = 0;
	static int Month = 0;
	int i, j;
	if (month(0) % 3 == 0 && !is(LOOKBACK)) {

		for (i = 0; i < N; i++) {
			Means[i] = Moment(Returns[i], LookBack, 1);
			for (j = 0; j < N; j++)
				Covariances[N * i + j] = Covariance(Returns[i], Returns[j], LookBack);
		}

		BestVariance = markowitz(Covariances, Means, N, 0.5);
		Month++;

	}
	static var Return, ReturnN, ReturnMax, ReturnBest, ReturnMin;
	if (is(LOOKBACK)) {
		Month = 0;
		ReturnN = ReturnMax = ReturnBest = ReturnMin = 0;
	}

	if (BestVariance > 0) {
		for (Return = 0, i = 0; i < N; i++) Return += (Returns[i])[0] / N; // 1/N 
		ReturnN = (ReturnN + 1) * (Return + 1) - 1;

		markowitzReturn(Weights, 0);	// minima variancia
		for (Return = 0, i = 0; i < N; i++) Return += Weights[i] * (Returns[i])[0];
		ReturnMin = (ReturnMin + 1) * (Return + 1) - 1;

		markowitzReturn(Weights, 1);	// maximo retorno
		for (Return = 0, i = 0; i < N; i++) Return += Weights[i] * (Returns[i])[0];
		ReturnMax = (ReturnMax + 1) * (Return + 1) - 1;

		markowitzReturn(Weights, BestVariance); // max Sharpe
		for (Return = 0, i = 0; i < N; i++) Return += Weights[i] * (Returns[i])[0];
		ReturnBest = (ReturnBest + 1) * (Return + 1) - 1;

		set(PLOTNOW);
		PlotHeight1 = 300;
		PlotWidth = 1000;
		ColorUp = ColorDn = ColorWin = ColorLoss = ColorEquity = ColorDD = 0;
		plot("1/N", 100 * ReturnN, AXIS2, BLACK);
		plot("Max Sharpe", 100 * ReturnBest, AXIS2, GREEN);
		plot("Retorno Máximo", 100 * ReturnMax, AXIS2, RED);
		plot("Min Variancia", 100 * ReturnMin, AXIS2, BLUE);

	}


#endif // BACKTEST

	if (is(EXITRUN)) {




#ifdef CARTEIRA
		int i, j;
		for (i = 0; i < N; i++) {
			Means[i] = Moment(Returns[i], LookBack, 1);
			for (j = 0; j < N; j++)
				Covariances[N * i + j] = Covariance(Returns[i], Returns[j], LookBack);
		}

		var PesoMaximoAtivo = 0.5;
		var BestVariancia = markowitz(Covariances, Means, N, PesoMaximoAtivo);
		var MinVariancia = markowitzVariance(0, 0);
		markowitzReturn(Weights, MinVariancia + VFactor / 100. * (BestVariancia - MinVariancia));

		for (i = 0; i < N; i++) {
			asset(Names[i]);
			MarginCost = priceClose(0) / LEVERAGE;
			int Position = TotalCapital * Weights[i] / MarginCost;
			if (Position > 0)
				printf("\n%s:  %d Acoes a R$%.0f", Names[i], Position, priceClose(0));
		}

#endif // CARTEIRA
#ifdef FRONTEIRA
		for (i = 0; i < N; i++) {
			Means[i] = Moment(Returns[i], LookBack, 1);
			for (j = 0; j < N; j++)
				Covariances[N * i + j] = Covariance(Returns[i], Returns[j], LookBack);
		}


		var BestV = markowitz(Covariances, Means, N, 0.0);
		var MinV = markowitzVariance(0, 0);
		var MaxV = markowitzVariance(0, 1);

		PlotScale = 8;
		PlotHeight1 = 300;
		int Steps = 50;
		for (i = 0; i < Steps; i++) {
			var V = MinV + i * (MaxV - MinV) / Steps;
			var R = markowitzReturn(0, V);
			plotBar("Frontier", i, V, 100 * R, LINE | LBL2, BLACK);
		}
		plotGraph("Max Sharpe", (BestV - MinV) * Steps / (MaxV - MinV),
			100 * markowitzReturn(0, BestV), SQUARE, GREEN);

#endif // FRONTEIRA




	}
}




