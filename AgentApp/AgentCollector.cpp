/*
*	AgentApp - Simulation app
*
*   AgentCollector class implementation file
*
*	by Oguz Demir / August 2015
*/

#include "stdafx.h"
#include "AgentCollector.h"
#include <iostream>
using namespace std;

//Collector creates set of agents with given conditions.
//Determines the static variables which are used for every agent.
AgentCollector::AgentCollector(int n,double givenK,double givenV, double givenM, int xbins, int *xbinsloc, int ybins, int *ybinsloc, int *binsdensity)
{
	
	agentList = new Agent*[n];
	for (int i = 0; i < n; i++)
	{
		//Agents are created n times, randomly distributed in given dimensions.
		agentList[i] = new Agent(xbinsloc,ybinsloc,binsdensity,xbins,ybins);
	}
	//Holding bins locations and numbers in the object not to get it again and again from functions
	agentList[0]->k = givenK;
	agentList[0]->vmax = givenV;
	agentList[0]->maxmass = givenM;
	numberOfAgents = n;
	numberOfXbins = xbins;
	numberOfYbins = ybins;
	numberOfBins = xbins*ybins;
	locationXbins = xbinsloc;
	locationYbins = ybinsloc;
	binsTransaction = new int*[numberOfBins];
	for (int i = 0; i < numberOfBins; i++)
	{
		binsTransaction[i] = new int[numberOfBins];
	}
	//Setting all entries to 0
	for (int i = 0; i < numberOfBins; i++)
	{
		for (int j = 0; j < numberOfBins; j++)
			binsTransaction[i][j] = 0;
	}
}

//In destructor, dynamic allocations are deleted to prevent from memory leak.
AgentCollector::~AgentCollector()
{
	for (int i = 0; i < numberOfAgents; i++)
	{
		delete agentList[i];
	}
	delete[] agentList;

	for (int i = 0; i < numberOfBins; i++)
	{
		delete [] binsTransaction[i];
	}
	delete[] binsTransaction;
}

//Implementation of processAll
void AgentCollector::processAll(int time, int agentStep, int timeStep, double **agentTraceX, double **agentTraceY, double **timeTraceX, double **timeTraceY, double radius )
{
	for (int t = 0; t < time + 1; t += 1)
	{
		for (int i = 0; i < numberOfAgents; i++)
		{
			if (i % agentStep == 0)
			{
				agentTraceX[i / agentStep][t] = agentList[i]->posX;
				agentTraceY[i / agentStep][t] = agentList[i]->posY;
			}
			if (t % timeStep == 0)
			{
				timeTraceX[t / timeStep][i] = agentList[i]->posX;
				timeTraceY[t / timeStep][i] = agentList[i]->posY;

			}
			agentList[i]->process(t, t + 1, binsTransaction, numberOfXbins, numberOfYbins, locationXbins, locationYbins);


		}
		
		if (t % timeStep == 0 && t > 0)
		{
			reportTransaction(t - timeStep, t);
		}
		
		reportCross(t, radius);

	}
} // end of processAll

//Implementation of transaction report
void AgentCollector::reportTransaction(int initialTime, int finalTime)
{
	bool no = true;
	cout << "\n\nBetween time " << initialTime << " " << finalTime ;
	for (int row = 0; row < numberOfBins; row++)
	{
		for (int col = 0; col < numberOfBins; col++)
		{
			if (col != row && binsTransaction[row][col] != 0)
			{
				cout <<"\n" << binsTransaction[row][col] << " agents has gone from " << row + 1 << ".bin to " << col + 1 << ".bin" << endl;
				no = false;
			}
				
		}
	}
	if (no)
	{
		cout << " no agent has changed bins." << endl;
	}
	//Setting all entries to 0 to make it ready for another report
	for (int i = 0; i <numberOfBins; i++)
	{
		for (int j = 0; j < numberOfBins; j++)
			binsTransaction[i][j] = 0;
	}
}

void AgentCollector::reportCross(int time,double radius)
{
	for (int a = 0; a < numberOfAgents; a++)
	{
		for (int b = a + 1; b < numberOfAgents; b++)
		{
			double x1 = agentList[a]->posX;
			double x2 = agentList[b]->posX;
			double y1 = agentList[a]->posY;
			double y2 = agentList[b]->posY;

			if ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) <= radius*radius)
				cout << "Agent " << a << ": ( " << x1 << " , " << y1 << " ) and Agent " << b << ": ( " << x2 << " , " << y2 << " ) is crossed " << endl;
		}
	}
}