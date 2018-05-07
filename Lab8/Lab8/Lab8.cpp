#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct node
{
	int value;
	node *child;
	node *sibling;
};

queue<int> *t1(const int r1[], const int n)
{
	queue<int> *r2;
	r2 = new queue<int>[n];

	for (int i = 0; i < n; ++i)
	{
		if (r1[i] != i)
		{
			r2[r1[i]].push(i);
		}
	}

	return r2;
}

node *t2(queue<int> *r2, const int index)
{
	node *r3 = new node;
	r3->value = index;
	r3->child = nullptr;
	r3->sibling = nullptr;

	if (r2[index].empty()) // if it has no children
	{
		return r3;
	}

	// arrive here if it has at least 1 children
	r3->child = t2(r2, r2[index].front());
	r2[index].pop();

	if (r2[index].empty()) // if it only has 1 child
	{
		return r3;
	}

	//arrive here if it has more children
	node *tmp = r3->child;
	while (!r2[index].empty())
	{
		tmp->sibling = t2(r2, r2[index].front());
		tmp = tmp->sibling;
		r2[index].pop();
	}

	return r3;
}

void preety_print(node *r3, int tabs)
{
	for (int i = 0; i < tabs; ++i)
	{
		cout << "\t";
	}

	cout << r3->value << endl;

	if (r3->child != nullptr)
	{
		preety_print(r3->child, tabs + 1);
	}

	if (r3->sibling != nullptr)
	{
		preety_print(r3->sibling, tabs);
	}
}

int main()
{
	int r1[10];
	r1[0] = 0;
	r1[1] = 0;
	r1[2] = 0;
	r1[3] = 1;
	r1[4] = 1;
	r1[5] = 1;
	r1[6] = 2;
	r1[7] = 2;
	r1[8] = 2;
	r1[9] = 2;

	queue<int> *r2 = t1(r1, 10);

	/*for (int i = 0; i < 10; ++i)
	{
		while (!r2[i].empty())
		{
			cout << r2[i].front() << " ";
			r2[i].pop();
		}
		cout << endl;
	}*/

	node *r3 = t2(r2, 0);

	preety_print(r3, 0);

	system("pause");

	delete r3;

	return 0;
}