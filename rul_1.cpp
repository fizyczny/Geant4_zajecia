#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <math.h>
#include <cmath>
#include <time.h>


using namespace std;

int wczytaj_liczbe()
	{
		int liczba=0;
		bool ok;
		do
		{
			cout << "podaj liczbe z przedzialu od 1 do 36" << endl;
			cin >> liczba;
			if (liczba>=1 && liczba<=36)
			{
				ok=cin.good();
			}
			if (!ok)
			{
				cout <<"wpisales bledna wartosc"<< endl;
				cin.clear();
			}
		} while (!ok);
		return liczba;
	}

int ruletka(int typ, int liczba)
{
	float N=1000000, stawka=1;  
	float wygrana ;
	int i,k, wylosowana;
	srand(time(0));
	for (i=1; i<=N; i++) 
	{
		wylosowana=((rand()%1000)/1000.)*(36+typ)+1; 
		if (wylosowana==liczba)
		{
			k++;
			wygrana+=35*stawka;
		}
		else
		{
			wygrana-=stawka;
		}
	}
	cout << "stopa zwrotu=" << (wygrana/(N*stawka))*100<<"%" <<endl;
}

int main()
{
	int liczba = wczytaj_liczbe();
	cout << "uczciwa"<< endl;
	ruletka(0,liczba);
	cout << "europejska" << endl;
	ruletka(1,liczba);
	cout << "amerykanska" << endl;
	ruletka(2,liczba);
	return 0;
}
