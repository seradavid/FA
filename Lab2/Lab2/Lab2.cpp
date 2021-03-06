#include "iostream"
#include "fstream"
#include "future"

using namespace std;

/* Bubblesort
	- has the most number of assignments
	- relatively few comparisons
	- in the best case it has linear complexity
	- in the worst case it is O(n^2)
*/

/* Selectionsort
	- very few assignments
	- every case has the worst number of comparisons
	- most number of operations performed
*/

/* Insertionsort
	- less assignemnts than bubblesort and more than selection
	- fewest number of comparisons
	- most efficient algorithm of the three
	- fewest number of operations
*/

ofstream avg("average.csv");
ofstream best("best.csv");
ofstream worst("worst.csv");

pair<int, int> bubblesort(int a[], int n)
{
	int nrAssignments = 0;
	int nrComparisons = 0;

	do
	{
		int newn = 0;

		for (int i = 0; i < n - 1; i++)
		{
			nrComparisons++;
			if (a[i] > a[i + 1])
			{
				swap(a[i], a[i + 1]);
				nrAssignments += 3;
				newn = i + 1;
			}
		}
		n = newn;
	} while (n);

	return make_pair(nrAssignments, nrComparisons);
}

pair<int, int> insertionsort(int a[], int n)
{
	int nrAssignments = 0;
	int nrComparisons = 0;

	int i = 1;

	while (i < n)
	{
		int x = a[i];
		nrAssignments++;
		int j = i - 1;

		while (j >= 0 && a[j] > x)
		{
			nrComparisons++;
			a[j + 1] = a[j];
			nrAssignments++;
			j--;
		}
		nrComparisons++;

		a[j + 1] = x;
		nrAssignments++;
		i++;
	}

	return make_pair(nrAssignments, nrComparisons);
}

pair<int, int> selectionsort(int a[], int n)
{
	int nrAssignments = 0;
	int nrComparisons = 0;

	for (int i = 0; i < n - 1; i++)
	{
		int minIndex = i;

		for (int j = i + 1; j < n; j++)
		{
			nrComparisons++;
			if (a[j] < a[minIndex])
			{
				minIndex = j;
			}
		}

		nrComparisons++;
		if (minIndex != i)
		{
			swap(a[i], a[minIndex]);
			nrAssignments += 3;
		}
	}

	return make_pair(nrAssignments, nrComparisons);
}

pair<int, int> calculateAverage(pair<int, int>(*sort)(int[], int), int n, int passes, int seed)
{
	srand(seed); // Set the seed for the random function

	pair<int, int> res;
	vector<future<pair<int, int>>> vec;
	int *a = new int[n];

	for (int j = 0; j < passes; j++)
	{
		for (int i = 0; i < n; i++)
		{
			a[i] = rand(); // generate a pseudo-random vector
		}

		// launch a new thread to sort the array
		vec.push_back(async(launch::async, sort, a, n));
	}

	for (int j = 0; j < passes; j++)
	{ // get the results of the sorts
		pair<int, int> r = vec[j].get();
		res.first += r.first;
		res.second += r.second;
	}

	delete a; // delete the unused variable

	// divide by the number of passes
	res.first /= passes;
	res.second /= passes;

	return res;
}

pair<int, int> calculateBest(pair<int, int>(*sort)(int[], int), int n)
{
	int *a = new int[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}

	pair<int, int> res = sort(a, n);

	delete a;

	return res;
}

pair<int, int> calculateWorst(pair<int, int>(*sort)(int[], int), int n)
{
	int *a = new int[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = n - i - 1;
	}

	pair<int, int> res = sort(a, n);

	delete a;

	return res;
}

int main()
{
	pair<int, int>(*bubble)(int[], int) = bubblesort;
	pair<int, int>(*insertion)(int[], int) = insertionsort;
	pair<int, int>(*selection)(int[], int) = selectionsort;

	vector<future<pair<int, int>>> vec;

	// Generate average case
	avg << "n,AssignmentsBubblesort,ComparisonsBubblesort,totalOperationsBubblesort,";
	avg << "n,AssignmentsInsertionsort,ComparisonsInsertionsort,totalOperationsInsertionsort,";
	avg << "n,AssignmentsSelectionsort,ComparisonsSelectionsort,totalOperationsSelectionsort" << endl;

	// Generate the best case
	best << "n,AssignmentsBubblesort,ComparisonsBubblesort,totalOperationsBubblesort,";
	best << "n,AssignmentsInsertionsort,ComparisonsInsertionsort,totalOperationsInsertionsort,";
	best << "n,AssignmentsSelectionsort,ComparisonsSelectionsort,totalOperationsSelectionsort" << endl;

	// Generate the worst case
	worst << "n,AssignmentsBubblesort,ComparisonsBubblesort,totalOperationsBubblesort,";
	worst << "n,AssignmentsInsertionsort,ComparisonsInsertionsort,totalOperationsInsertionsort,";
	worst << "n,AssignmentsSelectionsort,ComparisonsSelectionsort,totalOperationsSelectionsort" << endl;

	srand(time(0));

	for (int n = 100; n <= 10000; n += 100)
	{
		int seed = time(0) + rand();
		int passes = 5;
		vec.push_back(async(launch::async, calculateAverage, bubble, n, passes, seed));
		vec.push_back(async(launch::async, calculateAverage, insertion, n, passes, seed));
		vec.push_back(async(launch::async, calculateAverage, selection, n, passes, seed));

		vec.push_back(async(launch::async, calculateBest, bubble, n));
		vec.push_back(async(launch::async, calculateBest, insertion, n));
		vec.push_back(async(launch::async, calculateBest, selection, n));

		vec.push_back(async(launch::async, calculateWorst, bubble, n));
		vec.push_back(async(launch::async, calculateWorst, insertion, n));
		vec.push_back(async(launch::async, calculateWorst, selection, n));

		pair<int, int> bubbleAvg = vec[0].get();
		pair<int, int> inserionAvg = vec[1].get();
		pair<int, int> selectionAvg = vec[2].get();

		pair<int, int> bubbleBest = vec[3].get();
		pair<int, int> insertionBest = vec[4].get();
		pair<int, int> selectionBest = vec[5].get();

		pair<int, int> bubbleWorst = vec[6].get();
		pair<int, int> insertionWorst = vec[7].get();
		pair<int, int> selectionWorst = vec[8].get();

		avg << n << "," << bubbleAvg.first << "," << bubbleAvg.second << "," << bubbleAvg.first + bubbleAvg.second << ",";
		avg << n << "," << inserionAvg.first << "," << inserionAvg.second << "," << inserionAvg.first + inserionAvg.second << ",";
		avg << n << "," << selectionAvg.first << "," << selectionAvg.second << "," << selectionAvg.first + selectionAvg.second << endl;

		best << n << "," << bubbleBest.first << "," << bubbleBest.second << "," << bubbleBest.first + bubbleBest.second << ",";
		best << n << "," << insertionBest.first << "," << insertionBest.second << "," << insertionBest.first + insertionBest.second << ",";
		best << n << "," << selectionBest.first << "," << selectionBest.second << "," << selectionBest.first + selectionBest.second << endl;

		worst << n << "," << bubbleWorst.first << "," << bubbleWorst.second << "," << bubbleWorst.first + bubbleWorst.second << ",";
		worst << n << "," << insertionWorst.first << "," << insertionWorst.second << "," << insertionWorst.first + insertionWorst.second << ",";
		worst << n << "," << selectionWorst.first << "," << selectionWorst.second << "," << selectionWorst.first + selectionWorst.second << endl;

		vec.clear();
	}

	avg.close();
	best.close();
	worst.close();
	return 0;
}

