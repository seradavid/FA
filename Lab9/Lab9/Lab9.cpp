#include <iostream>
#include <fstream>
#include <queue>
#include <future>

using namespace std;

ofstream avg("average.csv");

bool print = true;

struct Node
{
	int value;
	Node *parent;
	int rank;

	Node()
	{
		value = 0;
		parent = nullptr;
		rank = 0;
	}

	Node(int value)
	{
		this->value = value;
		parent = this;
		rank = 0;
	}
};

struct Edge
{
	Node *n1;
	Node *n2;
	float cost;

	Edge(Node *n1, Node *n2, float cost)
	{
		this->n1 = n1;
		this->n2 = n2;
		this->cost = cost;
	}
};

class comparisonClass
{
public:
	bool operator() (Edge a, Edge b)
	{
		return a.cost > b.cost;
	}
};

void makeSet(Node *x)
{
	x->parent = x;
	x->rank = 0;
}

Node* find(Node *x, int &nrOp)
{
	if (x != x->parent)
	{
		nrOp += 2;
		x->parent = find(x->parent, nrOp);
	}
	return x->parent;
}

void merge(Node *n1, Node *n2, int &nrOp)
{
	Node *r1 = find(n1, nrOp);
	Node *r2 = find(n2, nrOp);

	if (r1 == r2)
	{
		return;
	}

	if (r1->rank > r2->rank)
	{
		r2->parent = r1;
		nrOp += 2;
	}
	else if (r1->rank == r2->rank)
	{
		r1->parent = r2;
		r2->rank++;
		nrOp += 3;
	}
	else
	{
		r1->parent = r2;
		nrOp += 3;
	}
}

priority_queue<Edge, vector<Edge>, comparisonClass>* generateQueue(int n, Node *nodes)
{
	srand((unsigned)time(0));

	priority_queue<Edge, vector<Edge>, comparisonClass> *pq = new priority_queue<Edge, vector<Edge>, comparisonClass>;

	for (int i = 0; i < n; ++i) // create n nodes
	{
		nodes[i].value = i;
		makeSet(nodes + i);
	}

	for (int i = 0; i < n - 1; ++i) // make sure we have a connected graph
	{
		Edge e = Edge(nodes + i, nodes + i + 1, rand() % 100);

		if (print) cout << e.n1->value << " " << e.n2->value << " " << e.cost << endl;

		pq->push(e);
	}

	int m = n;
	for (int i = 0; i < m; ++i) // create the rest of edges in a random order
	{
		Edge e = Edge(nodes + (rand() % n), nodes + (rand() % n), rand() % 100);

		if (print)cout << e.n1->value << " " << e.n2->value << " " << e.cost << endl;

		pq->push(e);
	}
	if (print)cout << endl;

	return pq;
}

int kruskal(int n, priority_queue<Edge, vector<Edge>, comparisonClass> *pq)
{
	int nrOp = 0;
	int i = 1;
	while (i < n) {
		Edge edge = pq->top();
		pq->pop();

		if (find(edge.n1, nrOp) != find(edge.n2, nrOp))
		{
			if (print) cout << edge.n1->value << "(" << edge.n1->rank << ")" << " " << edge.n2->value << "(" << edge.n1->rank << ")" << " " << edge.cost << endl;

			merge(edge.n1, edge.n2, nrOp);
			i++;
		}
	}

	return nrOp;
}

int calculateAverage(int n)
{
	Node *nodes = new Node[n];
	priority_queue<Edge, vector<Edge>, comparisonClass>* pq = generateQueue(n, nodes);

	int nrOp = kruskal(n, pq);

	delete nodes;
	delete pq;

	return nrOp;
}

void generateAverage(int passes)
{
	avg << "n,nrOp" << endl;

	vector<future<int>> res;
	int nrOp = 0;
	for (int n = 100; n <= 10000; n += 100)
	{
		nrOp = 0;
		for (int j = 0; j < passes; ++j) {
			res.push_back(async(launch::async, calculateAverage, n));
		}

		for (int j = 0; j < passes; ++j)
		{
			nrOp += res[j].get();
		}

		avg << n << "," << nrOp / passes << endl;

		res.clear();
	}
}

int main()
{
	if (print) {
		Node *unionTest = new Node[5];
		for (int i = 0; i < 5; i++)
		{
			unionTest[i].value = i;
			makeSet(unionTest + i);
		}
		int nrOp = 0;
		merge(unionTest + 1, unionTest + 3, nrOp);
		merge(unionTest + 0, unionTest + 2, nrOp);
		merge(unionTest + 2, unionTest + 4, nrOp);
		merge(unionTest + 3, unionTest + 4, nrOp);

		for (int i = 0; i < 5; i++)
		{
			cout << unionTest[i].value << "(rank: " << unionTest[i].rank << " repr: " << unionTest[i].parent->value << ")" << endl;
		}
		delete unionTest;

		Node *nodes = new Node[5];
		priority_queue<Edge, vector<Edge>, comparisonClass>* pq = generateQueue(5, nodes);

		cout << kruskal(5, pq) << endl;

		delete nodes;
		delete pq;
	}
	else
	{
		generateAverage(50);
	}

	system("pause");
	return 0;
}

