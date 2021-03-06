// Lab1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Profiler.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;

const int MAX_SIZE = 100;

ifstream fin("input.in");
ofstream fout("output.csv");

Profiler *profiler;

int power(int base, int pow)
{
	if (!base)
		return 0;

	int pt = pow;
	int p = 1;
	while (pow)
	{
		if (pow & 1)
		{
			p *= base;
			profiler->countOperation("multiplication", pt);
		}
		base *= base;
		profiler->countOperation("multiplication", pt);
		pow >>= 1;
		profiler->countOperation("shift", pt);
	}

	return p;
}

int main()
{
	int a[MAX_SIZE];
	int n;
	profiler = new Profiler();

	//for (int i = 0; i <= 10000; i += 100) 
	//{
		for (int j = 0; j <= 10000; j += 100)
		{
			auto start = chrono::high_resolution_clock::now();
			power(4576234, j);
			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
			//profiler->countOperation("time", duration);
			fout << j << ',' << duration << endl;
		}
	//}
	profiler->showReport();
	cin >> n;
	n = 100;

	FillRandomArray(a, n);
	for (int i = 0; i < n; i++)
		cout << a[i] << " ";

	fin.close();
	fout.close();
	return 0;
}

