#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <future>
#include <algorithm>

using namespace std;

ofstream fixedVertices("fixedVertices.csv");
ofstream fixedEdges("fixedEdges.csv");

class Comparator
{
public:
	bool operator()(pair<int, int>a, pair<int, int>b)
	{
		return a.second > b.second;
	}
};

#pragma region DIJKSTRA

int* dijkstra(vector<pair<int, int>> g[], int nrVertices, int root)
{
	int *cost = new int[nrVertices];
	/*
	for (int i = 1; i < nrVertices; ++i)
	{
		cost[i] = INT_MAX;
	}
	*/
	memset(cost, INT_MAX, nrVertices * sizeof(int));
	cost[root] = 0;

	priority_queue<pair<int, int>, vector<pair<int, int>>, Comparator> pq;
	pq.push(make_pair(root, 0));

	while (!pq.empty())
	{
		pair<int, int> node = pq.top();
		pq.pop();

		for (unsigned int i = 0; i < g[node.first].size(); ++i)
		{
			pair<int, int> newNode = g[node.first].at(i);
			if (cost[newNode.first] > cost[node.first] + newNode.second)
			{
				cost[newNode.first] = cost[node.first] + newNode.second;
				pq.push(make_pair(newNode.first, cost[newNode.first]));
			}
		}
	}

	return cost;
}

#pragma endregion

#pragma region BFS

void bfs(vector<int> g[], int nrVertices)
{
	queue<int> q;
	bool *visited = new bool[nrVertices];
	memset(visited, 0, nrVertices * sizeof(bool));

	for (int i = 0; i < nrVertices; ++i)
	{
		if (!visited[i])
		{
			visited[i] = true;
			q.push(i);

			while (!q.empty())
			{
				int node = q.front();
				q.pop();

				for (unsigned int i = 0; i < g[node].size(); ++i)
				{
					int newNode = g[node].at(i);
					if (!visited[newNode])
					{
						visited[newNode] = true;
						q.push(newNode);
					}
				}
			}
		}
	}

	delete visited;
}

#pragma endregion

#pragma region DFS

int dfs(vector<int> g[], int nrVertices)
{
	int nrOp = 0;

	stack<int> s;
	bool *visited = new bool[nrVertices];
	memset(visited, 0, nrVertices * sizeof(bool));

	for (int i = 0; i < nrVertices; ++i)
	{
		nrOp++;
		if (!visited[i])
		{
			visited[i] = true;
			s.push(i);

			while (!s.empty())
			{
				int node = s.top();
				s.pop();

				for (unsigned int i = 0; i < g[node].size(); ++i)
				{
					nrOp++;
					int newNode = g[node].at(i);
					if (!visited[newNode])
					{
						visited[newNode] = true;
						s.push(node);
						s.push(newNode);
						break;
					}
				}
			}
		}
	}

	delete visited;

	return nrOp;
}

void defese(vector<int> g[], bool visited[], int root)
{
	for (unsigned int i = 0; i < g[root].size(); ++i)
	{
		int newNode = g[root].at(i);
		if (!visited[newNode])
		{
			visited[newNode] = true;
			defese(g, visited, newNode);
		}
	}
}

void dfsRecursive(vector<int> g[], int nrVertices)
{
	bool *visited = new bool[nrVertices];
	memset(visited, 0, nrVertices * sizeof(bool));

	for (int i = 0; i < nrVertices; ++i)
	{
		if (!visited[i])
		{
			visited[i] = true;
			defese(g, visited, i);
		}
	}

	delete visited;
}

#pragma endregion

#pragma region TOPOLOGICAL SORT

void topological(vector<int> g[], bool visited[], int root, stack<int> *s)
{
	for (unsigned int i = 0; i < g[root].size(); ++i)
	{
		int newNode = g[root].at(i);
		if (!visited[newNode])
		{
			visited[newNode] = true;
			topological(g, visited, newNode, s);
		}
	}

	s->push(root);
}

void topologicalSort(vector<int> g[], int nrVertices)
{
	stack<int> *s = new stack<int>();
	bool *visited = new bool[nrVertices];
	memset(visited, 0, nrVertices * sizeof(bool));

	for (int i = 0; i < nrVertices; ++i)
	{
		if (!visited[i])
		{
			visited[i] = true;
			topological(g, visited, i, s);
		}
	}

	for (int i = 0; i < nrVertices; ++i)
	{
		cout << s->top() << " ";
		s->pop();
	}
	cout << endl;

	delete visited;
}

#pragma endregion

#pragma region TARJAN

void tarjanHelper(vector<int> g[], stack<int>s, int lowlink[], int index[], bool onStack[], int root, int &currentIndex)
{
	index[root] = currentIndex;
	lowlink[root] = currentIndex++;
	s.push(root);
	onStack[root] = true;

	for (unsigned int i = 0; i < g[root].size(); ++i)
	{
		int newNode = g[root].at(i);
		if (index[newNode] < 0)
		{
			tarjanHelper(g, s, lowlink, index, onStack, newNode, currentIndex);
			lowlink[root] = min(lowlink[root], lowlink[newNode]);
		}
		else if (onStack[newNode])
		{
			lowlink[root] = min(lowlink[root], index[newNode]);
		}
	}

	if (index[root] == lowlink[root])
	{
		int newNode = root;
		do {
			newNode = s.top();
			s.pop();
			onStack[newNode] = false;
			cout << newNode << " (" << lowlink[newNode] << ")" << endl;
		} while (newNode != root);
	}
}

void tarjan(vector<int> g[], int nrVertices)
{
	int *lowlink = new int[nrVertices];
	int *index = new int[nrVertices];
	bool *onStack = new bool[nrVertices];
	int currentIndex = 0;
	memset(lowlink, 0, nrVertices * sizeof(int));
	memset(index, -1, nrVertices * sizeof(int));
	memset(onStack, 0, nrVertices * sizeof(bool));

	stack<int>s;

	for (int i = 0; i < nrVertices; ++i)
	{
		if (index[i] < 0)
		{
			tarjanHelper(g, s, lowlink, index, onStack, i, currentIndex);
		}
	}

	delete lowlink;
	delete index;
	delete onStack;
}

#pragma endregion

#pragma region GENERATE AND OUTPUT DATA

vector<int> *generateGraph(int v, int e)
{
	srand((unsigned)time(0));

	vector<int> *g = new vector<int>[v];

	int i = 0;
	while (i < e)
	{
		int vertex1 = rand() % v;
		int vertex2 = rand() % v;

		if (find(g[vertex1].begin(), g[vertex1].end(), vertex2) != g[vertex1].end()) // there is link from v1 to v2
		{
			continue;
		}
		else // create the edge from vertex1 to vertex2
		{
			g[vertex1].push_back(vertex2);
			i++;
		}
	}

	return g;
}

void fixedNrOfVertices(int nrPasses)
{
	vector<future<int>> res;

	fixedVertices << "v,e,nrOp" << endl;

	int v = 100;
	for (int e = 1000; e <= 5000; e += 100)
	{
		int nrOp = 0;

		for (int j = 0; j < nrPasses; ++j) {
			res.push_back(async(launch::async, dfs, generateGraph(v, e), v));
		}

		for (int j = 0; j < nrPasses; ++j) {
			nrOp += res.at(j).get();
		}

		fixedVertices << v << "," << e << "," << nrOp / nrPasses << endl;

		res.clear();
	}

	fixedVertices.close();
}

void fixedNrOfEdges(int nrPasses)
{
	vector<future<int>> res;

	fixedEdges << "v,e,nrOp" << endl;

	int e = 9000;
	for (int v = 100; v <= 200; v += 10)
	{
		int nrOp = 0;

		for (int j = 0; j < nrPasses; ++j) {
			res.push_back(async(launch::async, dfs, generateGraph(v, e), v));
		}

		for (int j = 0; j < nrPasses; ++j) {
			nrOp += res.at(j).get();
		}

		fixedEdges << v << "," << e << "," << nrOp / nrPasses << endl;

		res.clear();
	}

	fixedEdges.close();
}

#pragma endregion

int main()
{
	/*
	DIJKSTRA
	vector<pair<int, int>> *g = new vector<pair<int, int>>[4];
	g[0].push_back(make_pair(1, 6));
	g[1].push_back(make_pair(0, 6));
	g[0].push_back(make_pair(2, 3));
	g[2].push_back(make_pair(0, 3));
	g[0].push_back(make_pair(3, 1));
	g[3].push_back(make_pair(0, 1));
	g[1].push_back(make_pair(2, 1));
	g[2].push_back(make_pair(1, 1));
	g[1].push_back(make_pair(3, 5));
	g[3].push_back(make_pair(1, 5));
	g[2].push_back(make_pair(3, 2));
	g[3].push_back(make_pair(2, 2));

	int *cost = dijkstra(g, 4, 0);

	for (int i = 0; i < 4; ++i)
	{
		cout << cost[i] << endl;
	}

	delete cost;
	*/

	/*
	TOPOLOGICAL SORT
	vector<int> *g = new vector<int>[4];
	g[0].push_back(1);
	g[1].push_back(2);
	g[2].push_back(3);

	topologicalSort(g, 4);
	*/

	/*
	TARJAN
	*/

	/*
	DFS

	*/

	fixedNrOfVertices(10);
	fixedNrOfEdges(10);

	system("pause");

	return 0;
}
