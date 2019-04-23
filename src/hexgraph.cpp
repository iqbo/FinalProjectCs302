#include "hexgraph.h"

void Hexgraph::print(){
	for(int i = 0; i < nodes.size(); i++){
		cout << "Rank: " << i << endl;
		for(int j = 0; j < nodes[i].size(); j++){
			cout << "	Node: " << nodes[i][j]->number << " Wall: " << nodes[i][j]->wall << " Left: " << nodes[i][j]->left->number << " Right: " << nodes[i][j]->right->number;
			if(nodes[i][j]->opening == NULL) cout << "	Opening: Null" << endl;
			else cout << " Opening: " << nodes[i][j]->opening->number << endl;
		}
	}
}

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
				nodes[i][y]->opening = NULL;
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
			shell->number = i + 1;
		}
		else
		{
			shell->wall = 0;
			shell->number = i + 1;
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

int main(){
	bool wall1[6] = {0, 0, 0, 0, 0, 0};
	bool wall2[6] = {0, 1, 0, 1, 0, 1};
	bool wall3[6] = {1, 0, 1, 0, 1, 0};

	Hexgraph mygraph = Hexgraph(wall1, wall2, wall3);
	
	mygraph.print();
}
