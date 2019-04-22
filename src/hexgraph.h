#include <vector>
#include <math.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;



struct node {	//each node represents a wall
public:
	int number; //their number on the board easier for seeing positions of each wall
	int wall;	//if wall = 1, opening = NULL // 1 means there is a 
	node * opening = NULL;	//leads to higher rank
	node * left = NULL;			//node to the left 
	node * right = NULL;			//node to the right
	int rank;	//which shell this node belongs to
};

class Hexgraph {
public:
	//(what is the difference between shell and wall?
	Hexgraph(bool wall1[6], bool wall2[6], bool wall3[6]);
	void addShell(bool *walls);		//accepts a vector of pools to add a shell to the graph
	void checkOpenings();			//checks for possible openings from wall to wall
	//vector<shell *>;					//store shells or merge hexgraph and shell
	int maxRank;						//highest rank
	int findPath();						//finds a path (I am guessing this for finding the opening?)
	vector< vector<node*> >nodes;		//nodes[0] are all nodes in rank 0 shell (why is this 2-D?)
};

//goes throught the vector of nodes and sets available openings in the hexgraph
void Hexgraph::checkOpenings()
{
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		if (i == maxRank - 1)
		{
			return;
		}
		for (unsigned int y = 0; y < 6; y++)
		{
			if (nodes[i + 1][y]->wall == 1)
			{
				nodes[i][y]->opening = nodes[i + 1][y];
			}
			else
			{
				nodes[i][y]->opening = nullptr;
			}
		}
	}
}


//initiliazes graph, source node in center with opening to each of rank 0 node
Hexgraph::Hexgraph(bool wall1[6], bool wall2[6], bool wall3[6]) 
{
	//adds all three walls
	maxRank = 0;
	addShell(wall1);
	addShell(wall2);
	addShell(wall3);

	//increment max rank for each call to add shell
	//within the function maxrank
}

//constructs nodes for shell with appropiate connections
void Hexgraph::addShell(bool *walls) { //nodes[rank] = wall
	//traverse walls constructing one node per wall with appropiate fields
	vector <node*> wallss;
	maxRank++;
	//push new nodes to the wall
	for (unsigned int i = 0; i < 6; i++)
	{
		node* shell = new node;
		if (walls[i] == true)
		{
			shell->wall = 1;
			shell->number = i;
		}
		else
		{
			shell->wall = 0;
			shell->number = i;
		}
		wallss.push_back(shell);
	}
 
	for (unsigned int i = 0; i < 6; i++)
	{
		
		//set up the pointers to left and right walls
	
			if (i == 0)
			{
				wallss[i]->left = wallss[5];
				wallss[i]->right = wallss[1];
			}
			if (i >= 1 && i <= 4)
			{
				wallss[i]->left = wallss[i - 1];
				wallss[i]->right = wallss[i + 1];
			}
			if (i == 5)
			{
				wallss[i]->left = wallss[4];
				wallss[i]->right = wallss[0];
			}
		
	}
	nodes.push_back(wallss);
	checkOpenings(); //check for openings after each addition of a shelll 
	
}

//findPath
//searchs for a path from node[0][0] to highest ranking node
//rules: every node has left and ride field
//		 if a node has an opening filed it connects to a higher ranking node

// Probably BFS
int Hexgraph::findPath() {
	return 0;
}
