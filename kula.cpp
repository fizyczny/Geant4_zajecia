#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <math.h>
#include <cmath>
#include <time.h>


using namespace std;

int main()
{
	float N=100000, n=2, R=1;  //delkaruje wartosci parametrow
	float L, M, wym, V ; //tworze zmienne
	int i, j;
	srand(time(0));
	
	for (i=1; i<=N; i++) 
	{
		L=0;
		for (j=1;j<=n;j++)
		{
			wym=(((rand()%1000)/1000.)-0.5)*2*R; //generuje wsporzedna punktu
			L+=pow(wym,2); //dodaje do zmiennej kwadrat wsporzednej
		}
	
		if (pow(L,(1/n))<=R) //sprawdzam czy punkt lezy w kuli 
			{
				M++;
			}
	
		
	}
	V =(M*pow(2*R,n)/N);
	cout << "V=" << V ;
	return 0;
}
