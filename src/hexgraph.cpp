#include "hexgraph.h"
#include <map>

void Hexgraph::print(){
	for(int i = 0; i < nodes.size(); i++){
		cout << "Rank: " << i << endl;
		for(int j = 0; j < nodes[i].size(); j++){
			cout << "	Node: " << nodes[i][j]->number << " Wall: " << nodes[i][j]->wall << " Left: " << nodes[i][j]->left->number << " Right: " << nodes[i][j]->right->number;
			if(nodes[i][j]->opening == NULL) cout << " Opening: Null" << endl;
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
		shell->number = i;
		
		if (walls[i] == true)
		{
			shell->wall = 1;
		}
		else
		{
			shell->wall = 0;
		}

		shell->rank = maxRank-1;
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
//searchs for a path from node[0][playerPos] to highest ranking node
//rules: every node has left and ride field
//		 if a node has an opening filed it connects to a higher ranking node

// Standard old BFS (kind of)
int Hexgraph::findPath(int playerPos) {
	printf("playerPos: %d\n", playerPos);
	//Set all backedges to null and visited to false
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			nodes[i][j]->backedge = NULL;
			nodes[i][j]->visited = false;
			printf("is a node: %d\n", nodes[i][j]->rank);
		}
	}

	//queue <node*> q;
	//q.push(nodes[0][playerPos])

	multimap <int, node*> q;
	q.insert(make_pair(3, nodes[0][playerPos%6]));
	multimap <int, node*>::iterator top;


	while (!q.empty()) {
		top = q.begin();
		
		node * n = top->second;
		q.erase(top);

		if (n->rank == 2) {
			//print backedges
			for (node* N = n; N != NULL; N = N->backedge) {
				printf("%d %d\n", N->number, N->rank);
				if (N->rank == 0) return N->number;
			}
			//return 1;
		}

		if (! n->visited) {
//			if (not n->left->visited) {
				q.insert(make_pair(3 -n->rank, n->left));
				if (n->left->backedge == NULL)
					n->left->backedge = n;
//			}
//			if (not n->right->visited) {
				q.insert(make_pair(3 -n->rank, n->right));
				if (n->right->backedge == NULL)
					n->right->backedge = n;
//			}
			if (n->opening != NULL) {
//				if (not n->opening->visited) {
					q.insert(make_pair(3 -n->opening->rank, n->opening));
					if (n->opening->backedge == NULL)
						n->opening->backedge = n;
//				}
			}
		}
		n->visited = true;
	}

	printf("No path found\n");
	return -1;
}


/*
int main(){
	bool wall1[6] = {0, 0, 0, 0, 0, 0};
	bool wall2[6] = {0, 1, 0, 1, 0, 1};
	bool wall3[6] = {1, 0, 1, 0, 1, 0};

	Hexgraph mygraph = Hexgraph(wall1, wall2, wall3);

	mygraph.findPath(0);

	mygraph.print();
}
*/
