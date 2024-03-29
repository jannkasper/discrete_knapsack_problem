// cw4_EULER2.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <conio.h>
#include <algorithm>
#include <stdlib.h>
#include<stdio.h>
using namespace std;

//--------------------------------------------------------------------ZAPIS DO PLIKU--------------------------------------------------------------------
int saveArrayWyniki(double *wyniki[], int start, int krok, int liczbaPomiarow)
{
	fstream plik("plik-wyniki.txt", ios::out);
	if (plik.good())
	{
		plik << "wierzcholki \tDYNAMICZNE \tDOKLADNE \tZACHLANNE" << endl;
		for (int i = 0; i < liczbaPomiarow; i++)
		{
			plik << start << "\t";
			for (int j = 0; j < 3; j++)
			{

				plik << wyniki[j][i] << "\t";
				plik.flush();
			}
			start += krok;
			plik << endl;
		}
		
		plik.close();
		
	}
	return 0;
}
//--------------------------------------------------------------------DYNAMICZNE--------------------------------------------------------------------

int max(int a, int b) { return (a > b) ? a : b; }


int dynamic(int W, int wt[], int val[], int n)
{
	int i, w;
	int **K = new int*[n + 1];
	for (int m = 0; m < n + 1; m++) {
		K[m] = new int[W + 1];
	}

	// DYNAMICZNE UZUPELNIANE MACIERZY WZGLEDEM UKLADU 4 ROWNAN
	for (i = 0; i <= n; i++)
	{
		for (w = 0; w <= W; w++)
		{
			if (i == 0 || w == 0)
				K[i][w] = 0;
			else if (wt[i - 1] <= w)
				K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
			else
				K[i][w] = K[i - 1][w];
		}
	}
	int wynik = K[n][W];

	for (int i = 0; i < n + 1; i++)
		delete[] K[i];

	return wynik;
}

//--------------------------------------------------------------------DOKLADNE--------------------------------------------------------------------

int brutforce(int W, int wt[], int val[], int n)
{

	if (n == 0 || W == 0)														//JEZELI WAGA ZROWNANA DO 0 LUB BRAK ELEMENTOW KONCZY DODAWAC
		return 0;

	if (wt[n - 1] > W)															//JEZELI SIE NIE MISCIE NIE DODAJE ELEMENTU
		return brutforce(W, wt, val, n - 1);


	else return max(val[n - 1] + brutforce(W - wt[n - 1], wt, val, n - 1),		//POROWNUJE WARTOSC PRZY DODANIU I BEZ DODANIA Z EWENTULNYMI ROZWIAZANIAMI
		brutforce(W, wt, val, n - 1)
	);
}

//--------------------------------------------------------------------ZACHLANNE--------------------------------------------------------------------
struct przedmiot
{
	int waga;
	int wartosc;
	double stosunek;
};

int compare2(const void * a, const void * b)
{

	przedmiot *orderA = (przedmiot *)a;
	przedmiot *orderB = (przedmiot *)b;

	if (orderB->stosunek > orderA->stosunek)
	{
		return 1;
	}
	else if (orderB->stosunek < orderA->stosunek)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int zachlanny(int W, int w[], int p[], int n, int sum = 0, int waga = 0)
{
	int i;
	przedmiot *przedmioty = nullptr;
	przedmioty = new przedmiot[n];
	for (i = 0; i < n; i++)
	{
		//cout << p[i] << " " << w[i] << endl;
		przedmioty[i].stosunek = (double)p[i] / w[i];								//TWORZYMY STOSUNEK WARTOSCI DO WAGI KAZDEGO ELEMENTU
		//cout << "przedmioty[i].stosunek to: " << przedmioty[i].stosunek << endl;
		przedmioty[i].waga = w[i];
		przedmioty[i].wartosc = p[i];
	}

	qsort(przedmioty, n, sizeof(przedmiot), compare2);								//SORTUJEMY WZGLEDEM NAJWKORZYSTNIEJSZEGO STOSUNKU
	int a = 0;
	while (waga <= W && a < n)														//DODAJEMY ELEMENTY TAK DŁUGO AZ NIE PRZEKROCZA DOPUSZCZALNEJ WAGI
	{
		if (waga + przedmioty[a].waga <= W)
		{
			waga += przedmioty[a].waga;
			sum += przedmioty[a].wartosc;
		}
		a++;
	}
	delete[] przedmioty;
	//cout << "Zachlanny: " << sum << endl; //usuniete za ostatnim razem
	return sum;
}


//--------------------------------------------------------------------MAIN FUNCTION--------------------------------------------------------------------

int compare(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

int main()
{
	//printf_s("%lf",nasycenie[1]);
	int liczbaPomiarow = 10;
	int liczbaPowtorzen = 10;
	int startNode = 100;
	int krok = 100;
	int maxWaga = 200;
	int maxWartosc = 200;

	double **wyniki = new double*[3];
	for (int i = 0; i < 3; i++)
		wyniki[i] = new double[liczbaPomiarow];

	int pojemnosc;
	int liczbaWyrazow = startNode;

	double sumaPom[3] = { 0 };

	for (int petlaPomiaru = 0; petlaPomiaru < liczbaPomiarow; petlaPomiaru++)
	{
		pojemnosc = 0;


	
		srand(time(0));
		for (int petlaPowtorzen = 0; petlaPowtorzen < liczbaPowtorzen; petlaPowtorzen++)
		{
			sumaPom[3] = { 0 };
			int *weight = new int[liczbaWyrazow];
			int *volume = new int[liczbaWyrazow];
			for (int i = 0; i < liczbaWyrazow; i++)
			{
				weight[i] = rand() % maxWaga + 100;
				volume[i] = rand() % maxWartosc + 100;
				pojemnosc += weight[i];
			}
	
			pojemnosc = pojemnosc*0.25;
			qsort(weight, liczbaWyrazow, sizeof(int), compare);

			auto startTime = std::chrono::high_resolution_clock::now();
			cout << dynamic(pojemnosc, weight, volume, liczbaWyrazow) << "\t";
			auto finish = std::chrono::high_resolution_clock::now();
			sumaPom[0] += ((finish - startTime).count());

			startTime = std::chrono::high_resolution_clock::now();
			//cout << brutforce(pojemnosc, weight, volume, liczbaWyrazow) << "\t";
			finish = std::chrono::high_resolution_clock::now();
			sumaPom[1] += ((finish - startTime).count());

			startTime = std::chrono::high_resolution_clock::now();
			cout << zachlanny(pojemnosc, weight, volume, liczbaWyrazow) << endl;
			finish = std::chrono::high_resolution_clock::now();
			sumaPom[2] += ((finish - startTime).count());

			delete[] weight;
			delete[] volume;
		}

		for (int k = 0; k < 3; k++)
		{
			wyniki[k][petlaPomiaru] = sumaPom[k] / (liczbaPowtorzen*1000000);
			cout << wyniki[k][petlaPomiaru] << "\t";

		}
		cout << endl;
		liczbaWyrazow += krok;

	}
	printf_s("SIEMA\n");
	saveArrayWyniki(wyniki, startNode, krok, liczbaPomiarow);
	printf_s("SIEMA\n");

	return 0;
}




