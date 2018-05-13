#include <iostream>
#include <fstream>
#include <queue>
#include <future>

using namespace std;

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

Node* find(Node *x)
{
	if (x != x->parent)
	{
		x->parent = find(x->parent);
	}
	return x->parent;
}

void merge(Node *n1, Node *n2)
{
	Node *r1 = find(n1);
	Node *r2 = find(n2);

	if (r1 == r2)
	{
		return;
	}

	if (r1->rank > r2->rank)
	{
		r2->parent = r1;
	}
	else if (r1->rank == r2->rank)
	{
		r1->parent = r2;
		r2->rank++;
	}
	else
	{
		r1->parent = r2;
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
		Edge e = Edge(nodes + i, nodes + i + 1, rand());
		pq->push(e);
	}

	int m = 3 * n;
	for (int i = 0; i < m; ++i) // create the rest of edges in a random order
	{
		Edge e = Edge(nodes + (rand() % n), nodes + (rand() % n), rand());
		pq->push(e);
	}

	return pq;
}

void kruskal(int n, priority_queue<Edge, vector<Edge>, comparisonClass> *pq)
{
	int i = 1;
	while (i < n) {
		Edge edge = pq->top();
		pq->pop();

		if (find(edge.n1) != find(edge.n2))
		{
			merge(edge.n1, edge.n2);
			i++;
		}
	}
}

int calculateAverage(int n)
{
	Node *nodes = new Node[n];
	priority_queue<Edge, vector<Edge>, comparisonClass>* pq = generateQueue(n, nodes);

	kruskal(n, pq);

	delete nodes;
	delete pq;

	return 0;
}

void generateAverage(int passes)
{
	vector<future<int>> res;
	int nrOp = 0;
	for (int n = 100; n <= 10000; n += 100)
	{
		cout << n << endl;
		for (int j = 0; j < passes; ++j) {
			res.push_back(async(launch::async, calculateAverage, n));
		}

		for (int j = 0; j < passes; ++j)
		{
			nrOp += res[j].get();
		}

		res.clear();
	}
}

int main()
{
	generateAverage(10);

	system("pause");
	return 0;
}

