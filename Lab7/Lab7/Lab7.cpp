#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <future>
#include "tree.hpp"

using namespace std;

ofstream fout("avg.csv");

struct vals
{
	int left;
	int right;

	vals(int left, int right)
	{
		this->left = left;
		this->right = right;
	}
};

Tree<int> *generateTree(int n)
{
	Tree<int> *t = new Tree<int>();
	queue<vals> q;

	q.push(vals(0, n));
	while (!q.empty())
	{
		vals x = q.front();
		q.pop();

		if (x.left < x.right)
		{
			int k = (x.left + x.right) >> 1;
			t->insert(k);
			q.push(vals(x.left, k));
			q.push(vals(k + 1, x.right));
		}
	}

	return t;
}

int funct(int n) // calculates the number of operations
{
	srand((unsigned)time(0));

	Tree<int> *t = generateTree(n);
	int *array = new int[n];

	for (int i = 0; i < n; ++i)
	{
		array[i] = i;
	}

	while (!t->isEmpty())
	{
		int index = rand() % n;
		if (array[index] >= 0)
		{
			t->select(index);
			t->remove(index);
			array[index] = -1;
		}
	}

	int nrOp = t->getNrOp();

	delete t;
	delete array;

	return nrOp;
}

void generateAverage(int nrPasses)
{
	fout << "n,nrOp" << endl;
	vector<future<int>> v;
	for (int n = 100; n <= 10000; n += 100)
	{
		int nrOp = 0;
		for (int passes = 0; passes < nrPasses; ++passes)
		{
			v.push_back(async(launch::async, funct, n));
		}

		for (int passes = 0; passes < nrPasses; ++passes)
		{
			nrOp += v[passes].get();
		}
		v.clear();

		fout << n << "," << nrOp / nrPasses << endl;
	}

	fout.close();
}

void smallExample()
{
	Tree<int> *t = generateTree(15);
	char c;
	int x;
	
	while (true)
	{
		t->print();

		cin >> c;
		switch (c)
		{
		case 'q':
			return;
		case 's':
			cin >> x;
			cout << "The value of the node is: " << t->select(x)->value << endl;
			break;
		case 'd':
			cin >> x;
			t->remove(x);
			break;
		}
	}
}

int main()
{
	/*Tree<int> *t = generateTree(15);
	t->remove(4);
	t->remove(10);
	t->print();
	int x;
	cout << "Index to search: ";
	cin >> x;
	cout << "The value of the node is: " << t->select(x)->value << endl;
	delete t;
	system("pause");*/

	smallExample();

	//generateAverage(50);
	return 0;
}

