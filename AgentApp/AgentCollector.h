/*
*	AgentApp - Simulation app
*
*   AgentCollector class header file
*	AgentCollector holds the array of agents.
*	Also, number of bins and their locations are hold to observe bin transactions
*	binsTraction is a 2D array that holds the transaction such that (i,j) gives the count of agent which went from i to j.
*
*	processAll function make all the agents move in the duration of given time.
*	reportTransaction function reports bin changes counts of agents.
*	reportCross function reports the crossing agents.
*
*	by Oguz Demir / August 2015
*/
#pragma once
#include "Agent.h"
class AgentCollector
{
public:
	AgentCollector(int n,double givenK, double givenV,double givenM, int xbins, int *xbinsloc, int ybins, int *ybinsloc, int *binsdensity);
	~AgentCollector();
	void processAll(int time, int agentStep, int timeStep, double **agentTraceX, double **agentTraceY, double **timeTraceX, double **timeTraceY, double radius);
	void reportTransaction(int initialTime, int finalTime);
	void reportCross(int time,double radius);
	Agent **agentList;
	int numberOfAgents;
	int numberOfXbins;
	int numberOfYbins;
	int numberOfBins;
	int *locationXbins;
	int *locationYbins;
	int **binsTransaction;
};

