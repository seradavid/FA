#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <queue>
#include "tree.hpp"

using namespace std;

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

queue<vals> q;
Tree<int> t;

void generate(int n)
{
	q.push(vals(0, n));
	while (!q.empty())
	{
		vals x = q.front();
		q.pop();

		if (x.left < x.right)
		{
			int k = (x.left + x.right) >> 1;
			t.insert(k);
			q.push(vals(x.left, k));
			q.push(vals(k + 1, x.right));
		}
	}
}

int main()
{
	generate(16);
	t.print();
	system("pause");
	return 0;
}

