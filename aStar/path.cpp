// Manan Patel
// CMPSC 441
// Programming Assignment 1: A* Heuristic Search


#include "path.h"
#include <iostream>
#include "Node.h"
#include <vector>

using namespace std;

int main()
{
	int rows, columns, startR, startC, goalR, goalC,
		numObst, obstR, obstC, remove;

	int **path = 0;

	Node *startNode;
	Node *q;

	bool final = false;


	vector<pair<int, int>> moves;

	//get the info about rows and columns and start node, goal node, & obstacles
	cerr << "Enter the number of rows: " << endl;
	cin >> rows;
	cerr << "Enter the number of columns: " << endl;
	cin >> columns;
	cerr << "Enter coordinates for starting locations: " << endl;
	cin >> startR >> startC;
	cerr << "Enter coordinates for goal locations: " << endl;
	cin >> goalR >> goalC;
	cerr << "Number of obstacles?" << endl;
	cin >> numObst;


	//create the grid
	path = new int*[rows];
	for (int i = 0; i < rows; i++) {
		path[i] = new int[columns];
	}

	//initialize the grid with all 0's
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			path[i][j] = 0;
		}
	}

	//initialize the start node with 1 and goal node with 2
	path[startR][startC] = 1;
	path[goalR][goalC] = 2;


	//if there are obstacles
	if (numObst > 0)
	{
		//get the coordinates
		for (int i = 0; i < numObst; i++) {
			cerr << "Enter obstacle coordinates: " << endl;
			cin >> obstR >> obstC;
			path[obstR][obstC] = 3;
		}
	}

	//initialize the startNode with the values entered by the user
	startNode = new Node();
	startNode->nRow = startR;
	startNode->nCol = startC;
	startNode->g = 0;
	startNode->f = 0;
	
	//create openList and closedList vectors
	vector<Node*> openList, closedList;

	//push the startNode on the openList
	openList.push_back(startNode);

	//while the openList is not empty
	while (!openList.empty() && !final)
	{
				remove = 0;
				// q holds the node with the smallest f value
				q = openList[0];

				//traverse the openList 
				for (int i = 0; i < openList.size(); i++)
				{
					//check to see if there is a node with a lower f value then q's f value
					if (q->f >= openList[i]->f)
					{
						//if the current node's f value is less than q's f value  then set the current node to q
						q = openList[i];
						remove = i; // store this instance
					}
				}

				//remove the q with the lowest f value from the openList 
				openList.erase(openList.begin() + remove);


				//Generate q's successors and set their parents to q
				

				vector<Node*> nodes;
				//successor up
				if (q->nRow - 1 >= 0) {
					if (path[q->nRow - 1][q->nCol] != 3) {
						Node *up = new Node();
						up->nRow = q->nRow - 1;
						up->nCol = q->nCol;
						nodes.push_back(up);
					}
				}
				//successor left
				if (q->nCol - 1 >= 0) {
					if (path[q->nRow][q->nCol-1] != 3) {
						Node *left = new Node();
						left->nRow = q->nRow;
						left->nCol = q->nCol-1;
						nodes.push_back(left);
					}
				}

				//successor down
				if (q->nRow + 1 < rows) {
					if (path[q->nRow+1][q->nCol] != 3) {
						Node *down = new Node();
						down->nRow = q->nRow + 1;
						down->nCol = q->nCol;
						nodes.push_back(down);
					}
				}

				//successor right
				if (q->nCol + 1 < columns) {
					if (path[q->nRow][q->nCol + 1] != 3) {
						Node *right = new Node();
						right->nRow = q->nRow;
						right->nCol = q->nCol + 1;
						nodes.push_back(right);
					}
				}

				//go through each node 
				for (int k = 0; k < nodes.size(); k++)
				{
					//assign each node  with a parent node
					nodes[k]->parent = q;

					//print out the potential nodes
					cout << nodes[k]->nRow << " " << nodes[k]->nCol << endl;


					//if successor is the goal, stop the search
					if (nodes[k]->nRow == goalR  && nodes[k]->nCol == goalC)
					{
						final = true;
						while (nodes[k]->parent != NULL) {
							moves.push_back(make_pair(nodes[k]->nRow, nodes[k]->nCol));
							nodes[k] = nodes[k]->parent;
						}
						break;
					}


					//implement the following:
					//g(successor) = g(q) + 1
					//h(successor) = Manhattan distance from successor to goal
					//f(successor) = g(successor) + h(successor)


					nodes[k]->g = q->g + 1;
					nodes[k]->h = abs(nodes[k]->nRow - goalR) + abs(nodes[k]->nCol - goalC);
					nodes[k]->f = nodes[k]->g + nodes[k]->h;



					bool skip = true;


					//traverse the open list
					for (int i = 0; i < openList.size(); i++)
					{
						//if a node with the same position as successor is in the OPEN list
						if (nodes[k]->nRow == openList[i]->nRow && nodes[k]->nCol == openList[i]->nCol)
						{
							//which has a lower or equal f value than successor, skip this successor
							if (openList[i]->f <= nodes[k]->f)
							{
								skip = false;
								break;
							}
							skip = true;
						}
					}
					//traverse the closed list
					for (int i = 0; i < closedList.size(); i++)
					{
						//if a node with the same position as successor is in the CLOSED list
						if (nodes[k]->nRow == closedList[i]->nRow && nodes[k]->nCol == closedList[i]->nCol)
						{
							//which has a lower or equal f value than successor, skip this successor
							if (closedList[i]->f <= nodes[k]->f)
							{
								skip = false;
								break;
							}
							skip = true;
						}
					}

					//if skip is not true then push the successor node to the open list
					if (skip == true)
					{
						openList.push_back(nodes[k]);
					}
				}
				//push the q onto the closedList
				closedList.push_back(q);
			}

			//add the startnode at the back of the vector.
			moves.push_back(make_pair(startR,startC));

			cout << "-1 -1" << endl;

			//print the moves of the turtle, print moves vector backwards
			for (int i = moves.size() - 1; i >= 0; i--)
			{
				cout << moves[i].first<<" " << moves[i].second << endl;
			}
			cout << "-1 -1" << endl;

		

	/*
	while (startR != goalR || startC != goalC)
	{
		if (path[startR][startC] != path[goalR][goalC])
		{
			if (startR < goalR)
			{
				startR++;
			}
			else if (startR > goalR)
			{
				startR--;
			}
			else if (startC < goalC)
			{
				startC++;
			}
			else if (startC > goalC)
			{
				startC--;
			}
		}
		cout << startR << " " << startC << endl;
	}
	cout << "-1 -1" << endl;
	


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cerr << path[i][j] << " ";
		}
		cerr << endl;
	}
	*/


	system("pause");
	return 0;
}


