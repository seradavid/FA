#include "fstream"
#include "iostream"
#include "future"
#include "chrono"
#include "iomanip"

using namespace std;

ofstream avg("average.csv");
ofstream worst("worst.csv");

#pragma region Heapsort

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
		//nrOp++; // count the assignment
	}

	nrOp++; // count the compariosn
	if (r < n && a[r] > a[largest])
	{
		largest = r;
		//nrOp++; // count the assignment
	}

	//nrOp++; // count the comparison
	if (largest != i)
	{
		swap(a[i], a[largest]);
		nrOp += 3; // count the swap
		nrOp += maxHeapify(a, largest, n);
	}

	return nrOp;
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

#pragma endregion

#pragma region Insertionsort

int insertionSort(int a[], int l, int r)
{
	int nrOp = 0;
	int i = l + 1;

	while (i < r)
	{
		int x = a[i];
		nrOp++; // count the assignment
		int j = i - 1;

		while (j >= l && a[j] > x)
		{
			nrOp++; // count the comparison
			a[j + 1] = a[j];
			nrOp++; // count the assignment
			j--;
		}
		nrOp++; // count the last comparison

		a[j + 1] = x;
		nrOp++; // count the assignment
		i++;
	}

	return nrOp;
}

#pragma endregion

#pragma region Quicksort

pair<int, int> partition(int a[], int l, int r)
{
	int nrOp = 0;

	int x = a[r];
	nrOp++; // count the assignment
	int i = l - 1;
	for (int j = l; j < r; j++)
	{
		nrOp++; // count the comparison
		if (a[j] <= x)
		{
			i++;
			swap(a[i], a[j]);
			nrOp += 3; // count the swap
		}
	}
	swap(a[i + 1], a[r]);
	nrOp += 3; // count the swap

	return make_pair(i + 1, nrOp);
}

pair<int, int> randomizedPartition(int a[], int l, int r)
{
	int nrOp = 0;

	//srand(time(0));

	int random = (rand() % (r - l)) + l;
	swap(a[random], a[r]);
	nrOp += 3; // count the swap

	int x = a[r];
	nrOp++; // count the assignment
	int i = l - 1;
	for (int j = l; j < r; j++)
	{
		nrOp++; // count the comparison
		if (a[j] <= x)
		{
			i++;
			swap(a[i], a[j]);
			nrOp += 3; // count the swap
		}
	}
	swap(a[i + 1], a[r]);
	nrOp += 3; // count the swap

	return make_pair(i + 1, nrOp);
}

int quicksort(int a[], int l, int r)
{
	int nrOp = 0;

	if (r - l > 10)
	{
		pair<int, int> m = randomizedPartition(a, l, r);
		nrOp += m.second;
		nrOp += quicksort(a, l, m.first - 1);
		nrOp += quicksort(a, m.first + 1, r);
	}
	else
	{
		nrOp += insertionSort(a, l, r + 1);
	}

	return nrOp;
}

int randomSelect(int a[], int l, int r, int i)
{
	if (l == r)
	{
		return a[l];
	}

	pair<int, int> m = randomizedPartition(a, l, r);
	int k = m.first - l + 1;

	if (i == k)
	{
		return a[m.first];
	}
	else if(i < k)
	{
		return randomSelect(a, l, m.first - 1, i);
	}
	else
	{
		return randomSelect(a, m.first + 1, r, i - k);
	}
}

#pragma endregion

bool isSorted(int a[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (a[i] > a[i + 1])
			return false;
	}
	return true;
}

#pragma region Generation

int averageHeapsort(int n, int passes, int seed)
{
	int res = 0;
	vector<future<int>> vec;
	srand(seed);

	int *a = new int[passes * n];
	
	for (int j = 0; j < passes; j++) {
		for (int i = 0; i < n; i++)
		{
			a[(j * n) + i] = rand();
		}

		vec.push_back(async(launch::async, heapsort, a + (j * n), n));
	}

	for (int j = 0; j < passes; j++)
	{
		res += vec[j].get();
	}
	vec.clear();
	delete a;

	return res / passes;
}

int averageQuicksort(int n, int passes, int seed)
{
	int res = 0;
	vector<future<int>> vec;
	srand(seed);

	int *a = new int[passes * n];
	for (int j = 0; j < passes; j++) {
		for (int i = 0; i < n; i++)
		{
			a[(j * n) + i] = rand();
		}

		vec.push_back(async(launch::async, quicksort, a + (j * n), 0, n - 1));
	}

	for (int j = 0; j < passes; j++)
	{
		res += vec[j].get();
	}
	vec.clear();
	delete a;

	return res / passes;
}

void generateAverage(int passes)
{
	avg << "n,Heapsort,Quicksort" << endl;

	vector<future<int>> vec;
	for (int n = 1000; n <= 100000; n += 100)
	{
		int r = rand() + time(0);
		vec.push_back(async(launch::async, averageHeapsort, n, passes, r));
		vec.push_back(async(launch::async, averageQuicksort, n, passes, r));

		int avgHeap = vec[0].get();
		int avgQuick = vec[1].get();

		avg << n << "," << avgHeap << "," << avgQuick << endl;

		vec.clear();
	}
}

int averageWorstQuick(int n, int passes)
{
	int res = 0;
	vector<future<int>> vec;

	int *a = new int[passes * n];
	for (int j = 0; j < passes; j++) {
		for (int i = 0; i < n; i++)
		{
			a[(j * n) + i] = (rand() % 200) - 100;
		}

		vec.push_back(async(launch::async, quicksort, a + (j * n), 0, n - 1));
	}

	for (int j = 0; j < passes; j++)
	{
		res += vec[j].get();
	}
	vec.clear();
	delete a;

	return res / passes;
}

void generateWorst()
{
	worst << "n,Heapsort,Quicksort" << endl;

	for (int n = 1000; n <= 100000; n += 100)
	{
		int *a = new int[n];
		int worstHeap = 0;
		int worstQuick = 0;

		for (int i = 0; i < n; i++)
		{
			a[i] = i;
		}

		worstQuick += averageWorstQuick(n, 1);
		worstHeap += heapsort(a, n);

		worst << n << "," << worstHeap << "," << worstQuick << endl;

		delete a;
	}
}

#pragma endregion

void smallExample()
{
	int n = 15;
	int k;
	int a[15];
	int b[15];
	int c[15];

	for (int i = 0; i < n; i++)
	{
		a[i] = rand() % 100;
	}
	memcpy(b, a, n * sizeof(int));
	memcpy(c, a, n * sizeof(int));

	cout << "Initial array: ";
	for (int i = 0; i < n; i++)
	{
		cout << setw(5) << a[i] << " ";
	}
	cout << endl;

	heapsort(a, n);
	quicksort(b, 0, n - 1);

	cout << "Heapsort:      ";
	for (int i = 0; i < n; i++)
	{
		cout << setw(5) << a[i] << " ";
	}
	cout << endl;

	cout << "Quicksort:     ";
	for (int i = 0; i < n; i++)
	{
		cout << setw(5) << b[i] << " ";
	}
	cout << endl;

	cout << "Index for quickselect: ";
	cin >> k;

	cout << endl << "Quickselect value: ";
	cout << randomSelect(c, 0, n - 1, k + 1) << endl;

	system("pause");
}

void problem()
{
	
}

int main()
{
	//generateAverage(30);
	//generateWorst();
	//smallExample();

	//system("pause");
	return 0;
}

