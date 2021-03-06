#include <stdlib.h>
#include <stdio.h>
#include <future>
#include <fstream>
#include <iostream>

using namespace std;

ofstream average("average.csv");

struct Hashtable
{
	int size;
	int *array;
	bool *empty;

	Hashtable() // constructor
	{
		size = 10007;
		array = new int[size];
		empty = new bool[size];

		for (int i = 0; i < size; ++i)
		{
			array[i] = 0;
			empty[i] = true;
		}
	}

	Hashtable(int n) // constructor
	{
		size = n;
		array = new int[size];
		empty = new bool[size];

		for (int i = 0; i < size; ++i)
		{
			array[i] = 0;
			empty[i] = true;
		}
	}

	~Hashtable() // destructor
	{
		delete array;
		delete empty;
	}

	int insert(int value)
	{
		for (int i = 0; i < size; ++i)
		{
			int index = (abs(value) + i * i) % size;
			if (empty[index])
			{
				array[index] = value;
				empty[index] = false;
				return index;
			}
		}
		return -1;
	}

	int search(int key)
	{
		int nrOp = 0;
		for (int i = 0; i < size; ++i)
		{
			int index = (abs(key) + i * i) % size;
			if (empty[index])
			{
				//cout << "element not found  (" << nrOp << " rehashes)" << endl;
				return nrOp;
			}
			if (!empty[index] && array[index] == key)
			{
				//cout << "element found (" << nrOp << " rehashes)" << endl;
				return nrOp;
			}
			else
			{
				++nrOp;
			}
		}
		return nrOp;
	}
};

struct Results
{
	double averageFound = 0;
	int maxFound = 0;
	double averageNotFound = 0;
	int maxNotFound = 0;
};

Results avg(double fillRate) // calculate the average for one hashtable
{
	srand((unsigned)time(0));

	Hashtable h;
	Results r;
	int newn = h.size * fillRate;

	for (int i = 0; i < newn; ++i) // insert n * fillrate elements
	{
		h.insert(rand());
	}

	int i = 0;
	while (i < 1500) // search 1500 elements found in the hashtable
	{
		int index = rand() % h.size; // calculate an index
		if (h.empty[index] == 0) // if the element at the index is filled with a value
		{
			int tmp = h.search(h.array[index]); // search for it
			r.maxFound = max(r.maxFound, tmp);
			r.averageFound += tmp;
			++i;
		}
	}
	r.averageFound /= 1500.0; // calculate the average

	i = 0;
	while (i < 1500) // search for 1500 elements not found in the table
	{
		int value = -rand(); // generate a random negative number
		int tmp = h.search(value); // search for it
		r.maxNotFound = max(r.maxNotFound, tmp);
		r.averageNotFound += tmp;
		++i;
	}
	r.averageNotFound /= 1500.0; // calculate the average

	return r;
}

Results getNrOp(double fillRate, int passes) // calculate the average for multiple tables
{
	vector<future<Results>> res; // vector to hald the results
	Results r;

	for (int i = 0; i < passes; ++i) // generate number of passes tables
	{
		res.push_back(async(launch::async, avg, fillRate));
	}
	for (int i = 0; i < passes; ++i) // for each table get the results
	{
		Results tmp = res[i].get();
		r.averageFound += tmp.averageFound;
		r.maxFound = max(r.maxFound, tmp.maxFound);
		r.averageNotFound += tmp.averageNotFound;
		r.maxNotFound = max(r.maxNotFound, tmp.maxNotFound);
	}
	r.averageFound /= (double) passes; // divide by the number of passes
	r.averageNotFound /= (double) passes;
	res.clear();

	return r;
}

void generate(int passes) // generate the sample data and print it
{
	Results tmp;

	average << "FillingFactor,AvgEffortFound,MaxEffortFound,AvgEffortNotFound,MaxEffortNotFound" << endl;

	tmp = getNrOp(0.80, passes);
	average << 0.80 << "," << tmp.averageFound << "," << tmp.maxFound << "," << tmp.averageNotFound << "," << tmp.maxNotFound << endl;

	tmp = getNrOp(0.85, passes);
	average << 0.85 << "," << tmp.averageFound << "," << tmp.maxFound << "," << tmp.averageNotFound << "," << tmp.maxNotFound << endl;

	tmp = getNrOp(0.90, passes);
	average << 0.90 << "," << tmp.averageFound << "," << tmp.maxFound << "," << tmp.averageNotFound << "," << tmp.maxNotFound << endl;

	tmp = getNrOp(0.95, passes);
	average << 0.95 << "," << tmp.averageFound << "," << tmp.maxFound << "," << tmp.averageNotFound << "," << tmp.maxNotFound << endl;

	tmp = getNrOp(0.99, passes);
	average << 0.99 << "," << tmp.averageFound << "," << tmp.maxFound << "," << tmp.averageNotFound << "," << tmp.maxNotFound << endl;

	average.close();
}

void example()
{
	Hashtable h = Hashtable(10);
	char c;
	int x;
	while (true)
	{
		cin >> c;
		switch (c)
		{
		case 'q':
			exit(0);
		case 'p':
			for (int i = 0; i < h.size; i++)
			{
				cout << h.array[i] << " ";
			}
			cout << endl;
			break;
		case 'i':
			cin >> x;
			h.insert(x);
			break;
		case 's':
			cin >> x;
			h.search(x);
			break;
		default: break;
		}
		//cout << endl;
	}
}

int main()
{
	generate(100);
	//system("pause");
	//example();
	return 0;
}

