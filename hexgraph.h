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
	int wall;	//if wall = 1, opening = NULL
	node * opening = NULL;	//leads to higher rank
	node * left = NULL;			//node to the left 
	node * right = NULL;			//node to the right
	int rank;	//which shell this node belongs to
};

class Hexgraph {
public:
	//(what is the difference between shell and wall?
	Hexgraph(bool wall1[6], bool wall2[6], bool wall3[6]);
	void addShell(bool walls[6]);		//accepts a vector of pools to add a shell to the graph	
	//vector<shell *>;					//store shells or merge hexgraph and shell
	int maxRank;						//highest rank (Not 100% sure what rank is?)
	int findPath();						//finds a path (I am guessing this for finding the opening?)
	vector< vector<node*> >nodes;		//nodes[0] are all nodes in rank 0 shell (why is this 2-D?)
};

//initiliazes graph, source node in center with opening to each of rank 0 node
Hexgraph::Hexgraph(bool wall1[6], bool wall2[6], bool wall3[6]) 
{
	//adds all three walls 
	addShell(wall1);
	addShell(wall2);
	addShell(wall3);
	maxRank = 0;
	//increment max rank for each call to add shell
	//within the function maxrank
}

//constructs nodes for shell with appropiate connections
void Hexgraph::addShell(bool walls[6], int rank) { //nodes[rank] = wall
	//traverse walls constructing one node per wall with appropiate fields
	vector <node*> wall;
	wall.resize(6);
	//push new nodes to the wall
	for (unsigned int i = 0; i < 6; i++)
	{
		node* shell = new node;
		wall.push_back(shell);
	}
	maxRank++;  
	for (unsigned int i = 0; i < 6; i++)
	{
		//set up the pointers to left and right walls
		if (walls[i] == true)
		{
			if (i == 0)
			{
				wall[i]->left = wall[5];
				wall[i]->right = wall[1];
			}
			if (i >= 1 && i <= 4)
			{
				wall[i]->left = wall[i - 1];
				wall[i]->right = wall[i + 1];
			}
			if (i == 5)
			{
				wall[i]->left = wall[4];
				wall[i]->right = wall[0];
			}
		}
	}
	//can not do next part until we read in the next shell
	//after adding new shell, checking previous shell to set its node opening fields
	//increment max rank of wall
	
}

//findPath
//searchs for a path from node[0][0] to highest ranking node
//rules: every node has left and ride field
//		 if a node has an opening filed it connects to a higher ranking node

// Probably BFS
int Hexgraph::findPath() {

}
