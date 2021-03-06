#include "fstream"
#include "iostream"
#include "future"
#include "chrono"
#include "limits.h"
#include "iomanip"

using namespace std;

ofstream buildHeap("build.csv");
ofstream worst("worstBuild.csv");

inline int parent(int i)
{
	return (i - 1) >> 1;
}

inline int leftChild(int i)
{
	return (i << 1) + 1;
}

inline int rightChild(int i)
{
	return (i << 1) + 2;
}

int maxHeapify(int a[], int i, int n)
{
	int nrOp = 0;

	int largest = i;
	int l = leftChild(i);
	int r = rightChild(i);

	nrOp++; // count the comparison
	if (l < n && a[l] > a[largest])
	{
		largest = l;
		nrOp++; // count the assignment
	}

	nrOp++; // count the compariosn
	if (r < n && a[r] > a[largest])
	{
		largest = r;
		nrOp++; // count the assignment
	}

	nrOp++; // count the comparison
	if (largest != i)
	{
		swap(a[i], a[largest]);
		nrOp += 3; // count the swap
		nrOp += maxHeapify(a, largest, n);
	}

	return nrOp;
}

void minHeapify(int a[], int i, int n)
{
	int smallest = i;
	int l = leftChild(i);
	int r = rightChild(i);

	if (l < n && a[l] < a[smallest])
	{
		smallest = l;
	}
	if (r < n && a[r] < a[smallest])
	{
		smallest = r;
	}
	if (smallest != i)
	{
		swap(a[i], a[smallest]);
		maxHeapify(a, smallest, n);
	}
}

int buildMaxHeap(int a[], int n) // bottom-up
{
	int nrOp = 0;

	for (int i = n / 2 - 1; i >= 0; i--)
	{
		nrOp += maxHeapify(a, i, n);
	}

	return nrOp;
}

int extractTop(int a[], int &n)
{
	if (n < 1)
	{
		return INT_MIN;
	}

	int max = a[0];
	a[0] = a[n - 1];
	n--;
	maxHeapify(a, 0, n);

	return max;
}

int heapIncrease(int a[], int i, int value)
{
	int nrOp = 0;

	if (value < a[i])
	{
		return 0;
	}

	a[i] = value;
	nrOp++; // count the assignment
	while (i > 0 && a[parent(i)] < a[i])
	{
		nrOp++; // count the comparison
		swap(a[i], a[parent(i)]);
		nrOp += 3; // count the swap
		i = parent(i);
	}
	nrOp++; // count the final comparison

	return nrOp;
}

int insert(int a[], int &n, int value)
{
	n++;
	a[n] = INT_MIN;
	return heapIncrease(a, n, value);
}

int heapsort(int a[], int n)
{
	int nrOp = 0;

	nrOp += buildMaxHeap(a, n);
	for (int i = n - 1; i >= 1; i--)
	{
		swap(a[0], a[i]);
		nrOp += 3;
		nrOp += maxHeapify(a, 0, i);
	}

	return nrOp;
}

int calculateNrOpHeapsort(int n, int seed)
{
	srand(seed);

	int *a = new int[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = rand();
	}

	int nrOp = heapsort(a, n);

	delete a;

	return nrOp;
}

void calculateAvg(int passes)
{
	srand(time(0));

	vector<future<int>> res;

	for (int n = 100; n <= 10000; n += 50)
	{
		for (int p = 0; p < passes; p++)
		{
			res.push_back(async(launch::async, calculateNrOpHeapsort, n, rand()));
		}

		int nrOp;

		for (int p = 0; p < passes; p++)
		{
			nrOp += res[p].get();
		}

		res.clear();

		nrOp /= passes;

		cout << n << " " << nrOp << endl;
	}
}

void compareBuilds(int passes)
{
	srand(time(0) + rand());

	int nrOpTopDown;
	int nrOpBottomUp;

	buildHeap << "n,BottomUp,TopDown" << endl;

	for (int n = 100; n <= 10000; n += 10)
	{
		nrOpBottomUp = 0;
		nrOpTopDown = 0;

		for (int p = 0; p < passes; p++) {
			int *a = new int[n];
			int *b = new int[n];
			int j = -1;

			for (int i = 0; i < n; i++)
			{
				a[i] = rand();
				nrOpTopDown += insert(b, j, a[i]);
			}

			nrOpBottomUp += buildMaxHeap(a, n);

			delete a;
			delete b;
		}

		buildHeap << n << "," << nrOpBottomUp / passes << "," << nrOpTopDown / passes << endl;
	}

	buildHeap.close();
}

void worstCase()
{
	worst << "n,bottomUp,topDown" << endl;

	for (int n = 100; n <= 10000; n += 10)
	{
		int *a = new int[n];
		int *b = new int[n + 1];
		int nrOpBottomUp = 0;
		int nrOpTopDown = 0;
		int j = -1;

		for (int i = 0; i < n; i++)
		{
			a[i] = i;
			nrOpTopDown += insert(b, j, a[i]);
		}

		nrOpBottomUp += buildMaxHeap(a, n);

		delete a;
		delete b;

		worst << n << "," << nrOpBottomUp << "," << nrOpTopDown << endl;
	}

	worst.close();
}

void printAsTree(int a[], int n)
{
	int p = 0;
	for (int i = 0; i <= log2(n); i++)
	{
		for (int j = 0; j < (1 << i) && p < n; j++)
		{
			if (j == 0)
			{
				cout << setw((1 << ((int)log2(n) - i)) * 2) << a[p++];
			}
			else
			{
				cout << setw((1 << (((int)log2(n) - i) + 1)) * 2) << a[p++];
			}
		}
		cout << endl;
	}
}

void smallExample()
{
	int a[10];
	int b[10];
	int j = -1;

	for (int i = 0; i < 10; i++)
	{
		a[i] = rand() % ((i + 1) * (i + 1));
		//cout << a[i] << " ";
		insert(b, j, a[i]);
	}
	printAsTree(a, 10);
	for (int i = 0; i < 50; i++)
	{
		cout << "-";
	}
	cout << endl;

	buildMaxHeap(a, 10);

	printAsTree(a, 10);
	for (int i = 0; i < 50; i++)
	{
		cout << "-";
	}
	cout << endl;
	printAsTree(b, 10);
	/*
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << b[i] << " ";
	}
	cout << endl;*/
	system("pause");
}

void heapIncreaseMin(int a[], int i, int value)
{
	if (value > a[i])
	{
		return;
	}

	a[i] = value;
	while (i > 0 && a[parent(i)] > a[i])
	{
		swap(a[i], a[parent(i)]);
		i = parent(i);
	}
}

void heapInsertMin(int a[], int &n, int value)
{
	n++;
	a[n] = INT_MAX;
	heapIncreaseMin(a, n, value);
}

int heapRemoveMin(int a[], int &n)
{
	if (n < 0)
	{
		return INT_MIN;
	}

	int min = a[0];
	a[0] = a[n - 1];
	n--;
	minHeapify(a, 0, n);

	return min;
}

void problema()
{
	int n, k, nrOp = 0;
	cin >> n >> k;
	int *a = new int[n];
	int j = -1;

	for (int i = 0; i < n; i++)
	{
		int x;
		cin >> x;
		heapInsertMin(a, j, x);
	}

	while (j >= 0 && a[0] < k)
	{
		int m1 = heapRemoveMin(a, j);
		int m2 = heapRemoveMin(a, j);
		heapInsertMin(a, j, 2 * m1 + m2);
		nrOp++;
	}

	cout << nrOp << endl;

	delete a;
}

int main()
{
	//smallExample();
	// compareBuilds(50);
	// worstCase();
	problema();
	system("pause");
	return 0;
}