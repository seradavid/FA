#include <stdlib.h>
#include <queue>
#include <iostream>
#include <limits.h>
#include <time.h>
#include <fstream>

using namespace std;

ofstream avgConst1("avgConst1.csv");
ofstream avgConst2("avgConst2.csv");
ofstream avgConst3("avgConst3.csv");
ofstream avg("avg.csv");

struct Node
{
	int value;
	Node *next;

	Node()
	{
		value = 0;
		next = nullptr;
	}

	Node(int value)
	{
		this->value = value;
		next = nullptr;
	}
};

struct Heap
{
	Node* list[500];
	int size = 0;

	int sink(int index)
	{
		int nrOp = 0;
		int l = (index << 1) + 1;
		int r = (index << 1) + 2;
		int min = index;

		if (l < size && ++nrOp && list[min]->value > list[l]->value)
		{
			min = l;
		}
		if (r < size && ++nrOp && list[min]->value > list[r]->value)
		{
			min = r;
		}

		if (index != min)
		{
			swap(list[index], list[min]);
			nrOp += 3; // count the swap
			nrOp += sink(min);
		}

		return nrOp;
	}

	int insert(Node* n)
	{
		int nrOp = 0;

		list[size] = n;
		size++;

		int i = size - 1;
		while (i > 0 && ++nrOp && list[i]->value < list[(i - 1) >> 1]->value)
		{
			swap(list[i], list[(i - 1) >> 1]);
			nrOp += 3; // cout the swap
			i = (i - 1) >> 1;
		}

		return nrOp;
	}

	Node* peek()
	{
		return list[0];
	}

	bool isEmpty()
	{
		return size == 0;
	}

	int remove()
	{
		int nrOp = 0;

		if (list[0]->next != nullptr)
		{
			Node* tmp = list[0]->next;
			delete list[0];
			list[0] = tmp;

			nrOp++; // cout the assignment
			nrOp += sink(0);
			return nrOp;
		}

		swap(list[0], list[size - 1]);
		nrOp += 3; // count the swap
		size--;
		delete list[size];

		nrOp += sink(0);

		return nrOp;
	}

};

void freeAll();

Node* lists[500];
Node* res;

#pragma region Divide and Conquer

Node* merge(Node* a, Node* b)
{
	Node* result;

	if (a == 0)
	{
		return b;
	}
	if (b == 0)
	{
		return a;
	}

	if (a->value < b->value)
	{
		result = a;
		result->next = merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = merge(a, b->next);
	}

	return result;
}

Node* mergeLists(Node* l[], int k)
{
	k--;
	while (k > 0)
	{
		int i = 0;
		int j = k;

		while (i < j)
		{
			l[i] = merge(l[i], l[j]);

			i++;
			j--;

			if (i >= j)
			{
				k = j;
			}
		}
	}

	return l[0];
}

#pragma endregion

#pragma region Heap

int mergeWithHeap(int k)
{
	int nrOp = 0;

	Heap* h = new Heap;
	Node *tmp;
	res = new Node;
	tmp = res;

	for (int i = 0; i < k; i++)
	{
		nrOp += h->insert(lists[i]);
	}

	while (!h->isEmpty())
	{
		tmp->value = h->peek()->value;
		nrOp += h->remove();
		if (h->size > 0) {
			tmp->next = new Node;
			tmp = tmp->next;
		}
	}

	delete h;

	return nrOp;
}

#pragma endregion

#pragma region Generate

void generate(int n, int k)
{
	srand(time(0));

	int *a = new int[n];

	for (int j = 0; j < k; ++j) {
		for (int i = 0; i < n; ++i)
		{
			a[i] = rand();
		}

		sort(a, a + n);

		lists[j] = new Node;
		Node* tmp = lists[j];
		for (int i = 0; i < n; ++i)
		{
			tmp->value = a[i];
			if (i != n - 1) {
				tmp->next = new Node;
				tmp = tmp->next;
			}
		}
	}

	delete a;
}

void freeAll()
{
	for (int i = 0; i < 500; ++i)
	{
		if (lists[i] != nullptr)
		{
			Node* tmp;
			while (lists[i] != nullptr)
			{
				tmp = lists[i]->next;
				delete lists[i];
				lists[i] = tmp;
			}
		}
	}
}

void generateConstants()
{
	avgConst1 << "n,nrOp" << endl;
	avgConst2 << "n,nrOp" << endl;
	avgConst3 << "n,nrOp" << endl;

	int k1 = 5;
	int k2 = 10;
	int k3 = 100;

	for (int n = 20; n <= 2000; n += 20)
	{
		int nrOp = 0;
		generate(n, k1);
		nrOp += mergeWithHeap(k1);
		//freeAll();
		avgConst1 << n << "," << nrOp << endl;
	}

	for (int n = 10; n <= 1000; n += 10)
	{
		int nrOp = 0;
		generate(n, k2);
		nrOp += mergeWithHeap(k2);
		//freeAll();
		avgConst2 << n << "," << nrOp << endl;
	}

	for (int n = 1; n <= 100; n += 1)
	{
		int nrOp = 0;
		generate(n, k3);
		nrOp += mergeWithHeap(k3);
		//freeAll();
		avgConst3 << n << "," << nrOp << endl;
	}
}

void generateBigN()
{
	int n = 10000;

	avg << "k, nrOp" << endl;

	for (int k = 10; k <= 500; k += 10)
	{
		int nrOp = 0;
		generate(n, k);
		nrOp += mergeWithHeap(k);
		//freeAll();
		avg << k << "," << nrOp << endl;
	}
}

#pragma endregion

void print(Node* n)
{
	Node* tmp = n;
	while (tmp != nullptr)
	{
		cout << tmp->value << " ";
		tmp = tmp->next;
	}
	cout << endl;
}

void smallExample()
{
	int n = 5;
	int k = 3;
	generate(n, k);
	for (int j = 0; j < k; ++j)
	{
		Node*tmp = lists[j];
		while (tmp != nullptr)
		{
			cout << tmp->value << " ";
			tmp = tmp->next;
		}
		cout << endl;
	}
	mergeWithHeap(k);
	print(res);
	system("pause");
}

void problem(int n, int m)
{

	int nr = 0;
	int mn = 0;
	int **a = (int **) new int[n];
	for (int i = 0; i < n; ++i)
	{
		a[i] = new int[m];
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			a[i][j] = i + j - (m + n) / 2;
			//cout << a[i][j] << " ";
		}
		//cout << endl;
	}

	int i = 0;
	int j = m - 1;

	while (j >= 0 && i < n)
	{
		if (a[i][j] >= 0)
		{
			nr += mn;
			j--;
		}
		else
		{
			mn++;
			if (i == n - 1) 
			{
				nr += mn;
				j--;
			}
			else
			{
				i++;
			}
		}
	}

	cout << nr << endl;
}

int main()
{
	//for (int i = 0; i < 500; ++i)
	//{
	//	lists[i] = nullptr;
	//}

	//generateConstants();
	//generateBigN();

	//smallExample();

	problem(6, 8);
	system("pause");

	avgConst1.close();
	avgConst2.close();
	avgConst3.close();
	avg.close();
	//system("pause");
	return 0;
}

