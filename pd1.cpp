#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <math.h>
#include <cmath>


using namespace std;

int main()
{
	float t=20, l=8, k=100000;  //delkaruje wartosci parametrow
	float p, srodek, phi, x, pi; //tworze zmienne
	int i, j;
	
	for (i=1; i<=k; i++) 
	{
		srodek=((rand()%1000)/1000.)*5*t; //generuje wsporzedne srodka igly
		phi=((rand()%1000)/1000.)*M_PI; //generuje kat
		x=abs(l*cos(phi)/2); //licze dlugosc rzutu poziomego polowy igly
		for (j=0; j<=5; j++) //petla dla 5 przedzialow
		{
			if (j*t>=(srodek-x) && j*t<=(srodek+x)) //sprawdzam czy linia lezy w granicach rzutu poziomego igly 
			{
				p++;
			}
			
		}
	}
	pi=(2*l/(t*p/k)); 
	cout << "pi=" << pi;
	return 0;
}
