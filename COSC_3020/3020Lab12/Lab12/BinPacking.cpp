//  BinPacking.cpp

//  tom bailey   1730  27 apr 2015
//  Read a file of bin packing problems:
//    bin capacity, number of objects, object sizes
//  Calculate and report the number of bins required.
//  This calculation uses a non-optimal greedy algorithm.
//  The report is only an upper bound on the optimal number of bins.


#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

using namespace std;


//  Pre:  objSize is a collection of object sizes.
//        cap is the capacity of each bin.
//        Every object size is <= cap.
//  Post: the objects of objSize have been placed in bins.
//        The number of bins used is the size of
//        the vector remain.
//  This version uses a new bin for each object.
//  Bin information, stored in remain, is the 
//  remaining capacity.
void
fillBins(vector<int> objSize, int cap, vector<int> & remain)
{
	for (int obj : objSize)
	{
		//cout << "    inserting " << obj << endl;
		remain.push_back(cap - obj);
	}
}

void
bestFit(vector<int> objSize, int cap, vector<int> & remain)
{
	int best;
	int position;
	bool flag;
	for (int i = 0; i < objSize.size(); i++)
	{
		best = cap;
		position = 0;
		flag = false;
		for (int j = 0; j < remain.size(); j++)
		{
			if (remain[j] - objSize[i] < best && remain[j] - objSize[i] >=0)
			{
				best = remain[j] - objSize[i];
				position = j;
				flag = true;
			}
		}
		if (flag)
		{
			remain[position] = remain[position] - objSize[i];
		}
		else
		{
			remain.push_back(cap - objSize[i]);
		}
	}
}

void
firstFit(vector<int> objSize, int cap, vector<int> & remain)
{
	bool flag;
	int count;
	for (int obj : objSize)
	{
		flag = false;
		count = 0;
		for (int r : remain)
		{
			if (r - obj >= 0)
			{
				remain[count] = r - obj;
				flag = true;
				break;
			}
			count++;
		}
		if (!flag)
		{
			remain.push_back(cap - obj);
		}
	}
}

int
main()
{
	cout << "Use a new bin for each object" << endl;

	ifstream infile("BPProbs.txt");

	while (true)
	{
		// get the problem
		int binCap;
		infile >> binCap;
		if (!infile)
		{
			break;
		}
		int nObjects;
		infile >> nObjects;
		vector<int> oSize;
		for (int i = 0; i < nObjects; ++i)
		{
			int item;
			infile >> item;
			oSize.push_back(item);
		}

		cout << endl << "this problem:  bin capacity " << binCap
			<< ", " << oSize.size() << " objects" << endl << endl;

		// run scramble version;  the online version of the problem
		{
			vector<int> aSize(oSize);
			shuffle(begin(aSize), end(aSize), default_random_engine());
			vector<int>bins;
			fillBins(aSize, binCap, bins);
			cout << "shuffle then insert filled " << bins.size()
				<< " bins" << endl << endl;

			vector<int>bins2;
			bestFit(aSize, binCap, bins2);
			cout << "shuffle then best fit filled " << bins2.size()
				<< " bins" << endl << endl;

			vector<int>bins3;
			firstFit(aSize, binCap, bins3);
			cout << "shuffle then fisrt fit filled " << bins3.size()
				<< " bins" << endl << endl;
		}

		// run sort version;  the offline version of the problem
		{
			vector<int> aSize(oSize);
			sort(rbegin(aSize), rend(aSize));
			vector<int>bins;
			fillBins(aSize, binCap, bins);
			cout << "sort then insert filled " << bins.size()
				<< " bins" << endl << endl;

			vector<int>b2;
			bestFit(aSize, binCap, b2);
			cout << "sort then best fit filled " << b2.size()
				<< " bins" << endl << endl;

			vector<int>b3;
			firstFit(aSize, binCap, b3);
			cout << "sort then first fit filled " << b3.size()
				<< " bins" << endl << endl;
		}
	}

	return 0;
}