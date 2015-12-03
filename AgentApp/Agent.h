#pragma once
/*
*	AgentApp - Simulation app
*	
*   Agent class header file
*	Every agent holds its X and Y positions, X and Y velocities.
*	Static variables are used for computations,same for every agent.
*	process function make the agent move in given time interval.
*	findbin function determines the current bin of agent.
*	ode2func is the function that determines acceleration. 
*
*	by Oguz Demir / August 2015
*/
class Agent
{
public:
	//Static variables
	static double vmax;
	static double k;
	static int maxmass;

	Agent(int *xbinsloc, int *ybinsloc, int *binsdensity, int xbins, int ybins);
	~Agent();
	void process(int tin, int tfin, int** bins, int xbin, int ybin, int *locx, int *locy);
	int findbin(int x, int y, int *locx, int * locy);
	double ode2func(double t, double x, double v, int maxPosition);
	//Agent spec variables
	double m;
	double posX;
	double posY;
	double velX;
	double velY;
	
};

