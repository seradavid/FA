#include <stdlib.h>
#include <stdio.h>
#include <future>
#include <fstream>
#include <iostream>

using namespace std;

ofstream average("average.csv");

struct Hashtable
{
	int size = 10007;
	int array[10007];
	bool empty[10007];

	Hashtable()
	{
		for (int i = 0; i < size; ++i)
		{
			empty[i] = true;
		}
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
				cout << "element not found  (" << nrOp << " rehashes)" << endl;
				return nrOp;
			}
			if (!empty[index] && array[index] == key)
			{
				cout << "element found (" << nrOp << " rehashes)" << endl;
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
	int averageFound = 0;
	int maxFound = 0;
	int averageNotFound = 0;
	int maxNotFound = 0;
};

Results avg(double fillRate)
{
	srand((unsigned)time(0));

	Hashtable h;
	Results r;
	int newn = h.size * fillRate;

	for (int i = 0; i < newn; ++i)
	{
		h.insert(rand());
	}

	int i = 0;
	while (i < 1500)
	{
		int index = rand() % h.size;
		if (h.empty[index] == 0)
		{
			int tmp = h.search(h.array[index]);
			r.maxFound = max(r.maxFound, tmp);
			r.averageFound += tmp;
			++i;
		}
	}
	r.averageFound /= 1500;

	i = 0;
	while (i < 1500)
	{
		int value = -rand();
		int tmp = h.search(value);
		r.maxNotFound = max(r.maxNotFound, tmp);
		r.averageNotFound += tmp;
		++i;
	}
	r.averageNotFound /= 1500;

	return r;
}

Results getNrOp(double fillRate, int passes)
{
	vector<future<Results>> res;
	Results r;

	for (int i = 0; i < passes; ++i)
	{
		res.push_back(async(launch::async, avg, fillRate));
	}
	for (int i = 0; i < passes; ++i)
	{
		Results tmp = res[i].get();
		r.averageFound += tmp.averageFound;
		r.maxFound = max(r.maxFound, tmp.maxFound);
		r.averageNotFound += tmp.averageNotFound;
		r.maxNotFound = max(r.maxNotFound, tmp.maxNotFound);
	}
	r.averageFound /= passes;
	r.averageNotFound /= passes;
	res.clear();

	return r;
}

void generate(int passes)
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
	Hashtable h;
	char c;
	int x;
	while (true)
	{
		cin >> c;
		switch (c)
		{
		case 'q':
			exit(0);
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
	//generate(50);
	//system("pause");
	example();
	return 0;
}

