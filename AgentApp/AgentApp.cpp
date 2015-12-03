/*
*	AgentApp - Simulation app
*
*   Main method implementation
*	Uses matlab engine to visualize simulation results.
*
*	by Oguz Demir / August 2015
*/
#include "stdafx.h"
#include <Engine.h>
#include "AgentCollector.h"
#include <iostream>

#include <cmath>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string>
#include <sstream>  
//#pragma comment ( lib, "libmat.lib")
//#pragma comment ( lib, "libmx.lib")
//#pragma comment ( lib, "libeng.lib")
//#pragma comment ( lib, "libmex.lib")

using namespace std;

int inputint(string prompt, int* lowerbound, int* upperbound)
{
	int a;
	do
	{
		cout << "\n" << prompt << ":";
		cin >> a;
		if (lowerbound != NULL && a <= *lowerbound)
		{
			cout << "The number you entered should be greater than " << *lowerbound << ". Please try again." << endl;
			continue;
		}
		if (upperbound != NULL && a >= *upperbound)
		{
			cout << "The number you entered should be less than " << *upperbound << ". Please try again." << endl;
			continue;
		}
		return a;
	} while (true);
}
double inputdouble(string prompt, int* lowerbound, int* upperbound)
{
	double a;
	do
	{
		cout << "\n" << prompt << ":";
		cin >> a;
		if (lowerbound != NULL && a <= *lowerbound)
		{
			cout << "The number you entered should be greater than " << *lowerbound << ". Please try again." << endl;
			continue;
		}
		if (upperbound != NULL && a >= *upperbound)
		{
			cout << "The number you entered should be less than " << *upperbound << ". Please try again." << endl;
			continue;
		}
		return a;
	} while (true);
}
int _tmain(int argc, _TCHAR* argv[])
{

	int k = 1;
	int vmax = 20;
	int mmax = 20;

	int zero = 0; // To give lower bound to inputs
	int xDim = inputint("Please enter the width of experiment area", &zero, NULL);
	int yDim = inputint("Please enter the height of experiment area", &zero, NULL);
	int n = inputint("Please enter number of agent", &zero, NULL);
	int time = inputint("Please enter the execution time", &zero, NULL);
	int timestep = inputint("Please enter the timestep", &zero, NULL);
	int agentstep = inputint("Please enter the agentstep", &zero, NULL);
	double radius = inputint("Please enter the collision radius", &zero, NULL);
	int xbins = inputint("Please enter the number of X divisions", &zero, NULL);
	int ybins = inputint("Please enter the number of Y divisions", &zero, NULL);
	int numberofbins = xbins*ybins;
	
	//Creation of the array of X bins locations
	int *xbinsloc;
	xbinsloc = new int[xbins + 1];
	xbinsloc[0] = 0;
	xbinsloc[xbins] = xDim;
	for (int i = 1; i < xbins; i++)
	{
		string prompt = "Please enter the end position of X bin #";
		stringstream sstm;
		sstm << prompt << i << " ";
		string result = sstm.str();

		xbinsloc[i] = inputint(result, &xbinsloc[i - 1], &xDim);
	}	// end of creation xbinslocations

	//Creation of the array of Y bins locations
	int *ybinsloc;
	ybinsloc = new int[ybins + 1];
	ybinsloc[0] = 0;
	ybinsloc[ybins] = yDim;

	for (int i = 1; i < ybins; i++)
	{
		string prompt = "Please enter the end position of Y bin #";
		stringstream sstm;
		sstm << prompt << i << " ";
		string result = sstm.str();

		ybinsloc[i] = inputint(result, &ybinsloc[i - 1], &yDim);
	} // end of creation ybinslocations
	
	int *binsdensity = new int[numberofbins];
	if (numberofbins > 1)
	cout << "There is " << numberofbins << " bins. Please enter the bins densities out of 100." << endl;
	
	// Due to zero indexing, loop starts from 1, so i-1 is used when dereferencing.
	int total = 0;
	int minus1 = -1;
	bool finished = false;
	for (int i = 1; i < numberofbins + 1; i++)
	{
		if (finished)
		{
			binsdensity[i - 1] = 100;
			continue;
		}
		if (i == numberofbins)
		{
			cout <<"\n" << i << ". is the last bin, so remaining density is given to this bin." << endl;
			binsdensity[i - 1] = 100;
			break;
		}
		int remaining = 100 - total;
		string prompt = " .Please enter the density bin #";
		stringstream sstm;
		int remainingplus = remaining + 1;
		sstm << "Remaining density is " << remaining << prompt << i << " ";
		string result = sstm.str();

		int density = inputint(result, &minus1, &remainingplus);
		total += density;
		binsdensity[i - 1] = total;

		if (total == 100)
		{
			cout << "Density arrangement is finished. Remaining bins' density is set to zero" << endl;
			finished = true;
		}
	}

	for (int i = 0; i < numberofbins; i++)
	{
		cout << binsdensity[i] << "\t";
	}
	system("pause");
	AgentCollector *collector = new AgentCollector(n, k, vmax, mmax, xbins, xbinsloc, ybins, ybinsloc, binsdensity);
	
	//Total time to be processed
	int timesize = time + 1;
	
	

	//AgentTrace holds the position of determined agents position at every time step.
	//N-1 is the number of last agent, because indexing starts from 0.
	//If n = 100 and agentstep= 20, it holds 0,20,40,60,80 numbered agents positions.
	double **agentTraceX = new double*[n / agentstep];
	double **agentTraceY = new double*[n / agentstep];
	for (int i = 0; i < n / agentstep; i++)
	{
		agentTraceX[i] = new double[timesize];
		agentTraceY[i] = new double[timesize];
	}

	//xPositions and yPositions holds the everyagent positions in some time steps.
	//if execute contines 100 times, timesize 101 because of t=0 is also included.
	//If step is 20, 0,20,40,60,80,100 times will be shown. So +1 is added.
	double **timeTraceX = new double*[time / timestep + 1];
	double **timeTraceY = new double*[time / timestep + 1];
	for (int i = 0; i < time / timestep + 1; i++)
	{
		timeTraceX[i] = new double[n];
		timeTraceY[i] = new double[n];
	}


	collector->processAll(time, agentstep, timestep, agentTraceX, agentTraceY, timeTraceX, timeTraceY,radius);


	//Definging matlab engine.
	Engine *m_pEngine;
	m_pEngine = engOpen("null");

	
	for (int k = 0; k < n / agentstep; k++)
	{
		mxArray* XPOS = mxCreateDoubleMatrix(timesize, 1, mxREAL);
		memcpy((void *)mxGetPr(XPOS), (void *)agentTraceX[k], sizeof(double)*timesize);
		engPutVariable(m_pEngine, "agentPositionsX", XPOS);

		mxArray* YPOS = mxCreateDoubleMatrix(timesize, 1, mxREAL);
		memcpy((void *)mxGetPr(YPOS), (void *)agentTraceY[k], sizeof(double)*timesize);
		engPutVariable(m_pEngine, "agentPositionsY", YPOS);

		char *a = "figure('units', 'normalized', 'outerposition', [0 0 1 1]), hold on,";
		engEvalString(m_pEngine, a);
		for (int i = 1; i < xbins; i++)
		{

			stringstream sstmplot;
			sstmplot << "plot( [" << xbinsloc[i] << " " << xbinsloc[i] << "] , [ 0 " << yDim << " ] ),";
			string result = sstmplot.str();
			const char *cstr = result.c_str();
			engEvalString(m_pEngine, cstr);
		}
		for (int i = 1; i < ybins; i++)
		{

			stringstream sstmplot;
			sstmplot << "plot( [ 0 " << xDim << " ] , [ " << ybinsloc[i] << " " << ybinsloc[i] << " ] ),";
			string result = sstmplot.str();
			const char *cstr = result.c_str();
			engEvalString(m_pEngine, cstr);
		}
		stringstream sstmplot;
		sstmplot << "scatter(agentPositionsX,agentPositionsY,'b'), grid minor, title('TracingAgent = " << k*agentstep <<"' ), ";
		string result = sstmplot.str();
		const char *cstr = result.c_str();
		engEvalString(m_pEngine, cstr);
	}

	for (int k = 0; k <= timesize / timestep; k++)
	{
		mxArray* XPOS = mxCreateDoubleMatrix(n, 1, mxREAL);
		memcpy((void *)mxGetPr(XPOS), (void *)timeTraceX[k], sizeof(double)*n);
		engPutVariable(m_pEngine, "xPositions", XPOS);

		mxArray* YPOS = mxCreateDoubleMatrix(n, 1, mxREAL);
		memcpy((void *)mxGetPr(YPOS), (void *)timeTraceY[k], sizeof(double)*n);
		engPutVariable(m_pEngine, "yPositions", YPOS);

		char *a = "figure('units', 'normalized', 'outerposition', [0 0 1 1]), hold on,";
		engEvalString(m_pEngine, a);
		for (int i = 1; i < xbins; i++)
		{

			stringstream sstmplot;
			sstmplot << "plot( [" << xbinsloc[i] << " " << xbinsloc[i] << "] , [ 0 " << yDim << " ] ),";
			string result = sstmplot.str();
			const char *cstr = result.c_str();
			engEvalString(m_pEngine, cstr);
		}
		for (int i = 1; i < ybins; i++)
		{

			stringstream sstmplot;
			sstmplot << "plot( [ 0 " << xDim << " ] , [ " << ybinsloc[i] << " " << ybinsloc[i] << " ] ),";
			string result = sstmplot.str();
			const char *cstr = result.c_str();
			engEvalString(m_pEngine, cstr);
		}
		stringstream sstmplot;
		sstmplot << "scatter(xPositions,yPositions,'b'), grid minor, title('TracingAgent = " << k*timestep << "' ), ";
		string result = sstmplot.str();
		const char *cstr = result.c_str();
		engEvalString(m_pEngine, cstr);
	}

	//Deletion of dynamic allocations

	for (int i = 0; i < n / agentstep; i++)
	{
		delete [] agentTraceX[i];
		delete [] agentTraceY[i];
	}
	delete [] agentTraceX;
	delete [] agentTraceY;


	for (int i = 0; i < time / timestep + 1; i++)
	{
		delete[] timeTraceX[i];
		delete[] timeTraceY[i];
	}
	delete[]timeTraceX;
	delete[]timeTraceY;


	delete[] xbinsloc;
	delete[] ybinsloc;
	delete[] binsdensity;
	delete collector;
	system("pause");


	

	
	return 0;
}

