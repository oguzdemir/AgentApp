/*
*	AgentApp - Simulation app
*
*   Agent class implementation file
*
*	by Oguz Demir / August 2015
*/
#include "stdafx.h"
#include "Agent.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

double Agent::k = 1;
double Agent::vmax = 20;
int Agent::maxmass = 20;

Agent::Agent(int *xbinsloc, int *ybinsloc, int *binsdensity, int xbins, int ybins)
{
	//Constructor creates an agent.
	//First create a random number between 0-100
	int random100 = rand() % 100;
	
	// Densities are given increasing form to complete 100.
	// For example if there is 5 bin with densities 27,23,20,10,20 in total 100.
	// binsdensity array is : 27 50 70 80 100
	// So if the random number is between 70-80, it goes 4. bin
	int binnumber = xbins*ybins-1;
	for (int i = 0; i < xbins*ybins; i++)
	{
		if (random100 <= binsdensity[i])
		{
			binnumber = i;
			break;
		}
	}
	//Founding the row and col according to bin and # of bins.
	int foundxbin = binnumber % xbins;
	int foundybin = binnumber / xbins;

	//Positioning the agent randomly in the found bin
	posX = (rand() % (xbinsloc[foundxbin + 1] - xbinsloc[foundxbin])) + xbinsloc[foundxbin];
	posY = (rand() % (ybinsloc[foundybin + 1] - ybinsloc[foundybin])) + ybinsloc[foundybin];
	m = rand() % (int)maxmass;
	velX = 0;
	velY = 0;
}

//No de-allocation is need because agent holds no dynamically allocated property.
//So destructor is empty.
Agent::~Agent()
{
}
//Implementation of findbin function.
int Agent::findbin(int x, int y, int *locx, int * locy)
{
	//Checking current bin according to bin boundaries and object's position
	int a = -1;
	int b = -1;
	for (int i = 0; i < x; i++)
	{
		if (posX <= locx[i + 1])
		{
			a = i;
			break;
		}
			
	}
	for (int i = 0; i < y; i++)
	{
		if (posY <= locy[i + 1])
		{
			b = i;
			break;
		}
	}
	if (a == -1 || b == -1)
	{
		cout << "Critical problem with position " << endl;
		cout << "X's are ";
		for (int i = 0; i < x + 1; i++)
		{
			cout << "\t" << locx[i];
		}
		cout << "\n XPOS is " << posX << endl;

		cout << "Y's are ";
		for (int i = 0; i < y + 1; i++)
		{
			cout << "\t" << locy[i];
		}
		cout << "\n YPOS is " << posY;
	}
	return b*x + a;
} //end of findbin function

//Implementation of process function. (tin = time initial, tfin = time final.)
void Agent::process(int tin, int tfin, int** bins, int xbin, int ybin, int *locx, int *locy)
{
	//Finding bin before movement.
	int i = findbin(xbin, ybin, locx, locy);
	
	int maxpositionx = locx[xbin];
	int maxpositiony = locy[ybin];

	//Initializing variables for solving d.e.
	double xx = posX;
	double vx = velX;
	//Solving second order runge-kutta with step size 0.2 for X position
	double h = 0.2;
	for (double t = tin; t <= tfin; t += h)
	{
		double dx1 = h * vx;
		double dv1 = h * ode2func(t, xx, vx, maxpositionx);

		double dx2 = h * (vx + dv1 / 2);
		double dv2 = h * ode2func(t + h / 2, xx + dx1 / 2, vx + dv1 / 2, maxpositionx);

		double dx3 = h * (vx + dv2 / 2);
		double dv3 = h * ode2func(t + h / 2, xx + dx2 / 2, vx + dv2 / 2, maxpositionx);

		double dx4 = h * (vx + dv3);
		double dv4 = h * ode2func(t + h, xx + dx3, vx + dv3, maxpositionx);

		double dx = (dx1 + 2 * dx2 + 2 * dx3 + dx4) / 6;
		double dv = (dv1 + 2 * dv2 + 2 * dv3 + dv4) / 6;

		xx = xx + dx;
		vx = vx + dv;
	}

	//Initializing variables for solving d.e.
	double xy = posY;
	double vy = velY;

	//Solving second order runge-kutta with step size 0.2 for Y position
	for (double t = tin; t <= tfin; t += h)
	{
		double dx1 = h * vy;
		double dv1 = h * ode2func(t, xy, vy, maxpositiony);

		double dx2 = h * (vy + dv1 / 2);
		double dv2 = h * ode2func(t + h / 2, xy + dx1 / 2, vy + dv1 / 2, maxpositiony);

		double dx3 = h * (vy + dv2 / 2);
		double dv3 = h * ode2func(t + h / 2, xy + dx2 / 2, vy + dv2 / 2, maxpositiony);

		double dx4 = h * (vy + dv3);
		double dv4 = h * ode2func(t + h, xy + dx3, vy + dv3, maxpositiony);

		double dx = (dx1 + 2 * dx2 + 2 * dx3 + dx4) / 6;
		double dv = (dv1 + 2 * dv2 + 2 * dv3 + dv4) / 6;

		xy = xy + dx;
		vy = vy + dv;
	}

	posX = xx;
	//For now, agent is only moving horizontal.
	//posY = xy;

	
	
	//Assigning new velocities.
	velX = vx;
	velY = vy;

	//Remark: locx and locy holds the boundaries of bins. 
	//So first indexes holds the begining point of area, last indexes holds the end points.
	if (xx > locx[0] && xx < locx[xbin] && xy > locy[0] && xy < locy[ybin])
	{
		
		//Finding bin after movement.
		int j = findbin(xbin, ybin, locx, locy);

		//An agent crossed i to j.
		bins[i][j]++;
		return;
	}
	//cout << "Out of bounds";
	//If agent is out of bounds, it should stop.
	velX = 0;
	velY = 0;

	//If agent gone away through area borders, its position should be corrected to nearest boundary.
	if (posX < locx[0])
	{
		posX = locx[0];
	}
	if (posX > locx[xbin])
	{
		posX = locx[xbin];
	}
	if (posY < locy[0])
	{
		posY = locy[0];
	}
	if (posY > locy[ybin])
	{
		posY = locy[ybin];
	}
	//Finding bin after movement.
	int j = findbin(xbin, ybin, locx, locy);
	//An agent crossed i to j.
	bins[i][j]++;
} // end of process function

//Function that gives the acceleration(2.nd order d.e. will be solven)
double Agent::ode2func(double t, double x, double v,int maxPosition)
{
	double vref;

	if (x > (maxPosition / 2))
	{
		vref = vmax / (maxPosition / 2) * ((maxPosition / 2) - (x - maxPosition / 2));
	}
	else
	{
		vref = (vmax / (maxPosition / 2)) * x;
	}
	//cout << "V " << v <<  " and Vref: " << vref << endl;

	return -1 * k * (v - vref);
}