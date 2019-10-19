// VectorSortDriver.cpp
//  tom bailey   0920  21 oct 2011

// Create random sequences, sort the sequences, time the sort
//   routine.
// Factor out runCode and getRequest.
// Code for randomFill and write.
// Code for bubbleSort.
// Code for insertionSort, selectionSort
// Code for mergeSort.
// Code for orderedFill, reversedFill
// Code for quickSort.  

// tom bailey   1240  20 nov 2015
// Code for disorder fill
// Code for rand++ sort routine

#include "winTimer.h"

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::ostream;
#include <iomanip>
using std::setw;
#include <vector>
using std::vector;
#include <utility>
using std::swap;
#include <ctime>
using std::time;
#include <string>
using std::string;
#include <algorithm>
using std::is_sorted;


// pre:  0 <= n 
// post: sequence contains n randomly chosen
//       doubles from the range -n up to n.
void
randomFill(vector<double> & sequence, int n)
{
	sequence.clear();
	for (int i = 0; i < n; ++i)
	{
		double randReal = double(rand()) / (RAND_MAX + 1.0);
		randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);
		randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);
		randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);
		sequence.push_back(n * (randReal - 0.5) * 2.0);
	}
}


int
randInt(int low, int high)
{
	double randReal = double(rand()) / (RAND_MAX + 1.0);
	randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);
	randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);
	randReal = (double(rand()) + randReal) / (RAND_MAX + 1.0);

	return int(low + (high - low)*randReal);
}


// pre:  0 <= n 
// post: sequence contains n doubles, in order, starting
//       from -n with intervals randomly chosen from
//       the range 0.0 up to 4.0.
void
orderedFill(vector<double> & sequence, int n)
{
	sequence.clear();
	double current = -n;
	for (int i = 0; i < n; ++i)
	{
		double randReal = double(rand()) / (RAND_MAX + 1.0);
		current += randReal * 4.0;
		sequence.push_back(current);
	}
}


// pre:  0 <= n 
// post: sequence contains n doubles with disorder
//       of O(log n)
void
disorderedFill(vector<double> & sequence, int n)
{
	orderedFill(sequence, n);
	int i = 0;
	while (i*i < n)
	{
		swap(sequence.at(randInt(0, n)), sequence.at(randInt(0, n)));
		++i;
	}
}


// pre:  0 <= n 
// post: sequence contains n doubles, in reversed order, 
//       starting from n with intervals randomly chosen
//       from the range 0.0 up to 4.0.
void
reversedFill(vector<double> & sequence, int n)
{
	sequence.clear();
	double current = n;
	for (int i = 0; i < n; ++i)
	{
		double randReal = double(rand()) / (RAND_MAX + 1.0);
		current -= randReal * 4.0;
		sequence.push_back(current);
	}
}


// post: sequence has been written to outfile, one entry per line.
void
write(ostream & outfile, const vector<double> & sequence)
{
	for (size_t i = 0; i < sequence.size(); ++i)
	{
		outfile << setw(4) << i << "  " << sequence.at(i) << endl;
	}
}


// post: sequence is sorted from smallest to largest.
// Note: this is a slow sorting algorithm
void
slowSort(vector<double> & sequence)
{
	size_t swapAt;
	do
	{
		swapAt = sequence.size();
		for (size_t i = 1; i < sequence.size(); ++i)
		{
			if (sequence.at(i) < sequence.at(i - 1))
			{
				swapAt = i;
			}
		}
		if (swapAt < sequence.size())
		{
			swap(sequence.at(swapAt), sequence.at(swapAt - 1));
		}
	} while (swapAt < sequence.size());
}


// post: sequence is sorted from smallest to largest.
void
bubbleSort(vector<double> & sequence)
{
	for (size_t pass = 1; pass < sequence.size(); ++pass)
	{
		for (size_t pair = 1; pair < sequence.size(); ++pair)
		{
			if (sequence.at(pair) < sequence.at(pair - 1))
			{
				swap(sequence.at(pair), sequence.at(pair - 1));
			}
		}
	}
}


//// post: sequence is sorted from smallest to largest.
//void
//insertionSort(vector<double> & sequence)
//{
//	for (size_t source = 1; source < sequence.size(); ++source)
//	{
//		double nextValue = sequence.at(source);
//		size_t holeAt = source;
//		while (holeAt > 0 && nextValue < sequence.at(holeAt - 1))
//		{
//			sequence.at(holeAt) = sequence.at(holeAt - 1);
//			--holeAt;
//		}
//		sequence.at(holeAt) = nextValue;
//	}
//}


// post: *start up to *stop is sorted from smallest to largest.
// Note: type I is a bidirectional iterator; the entries accessed
//       via start and stop can be compared by operator<.
template<typename I>
void
insertionSortViaIterators(I start, I stop)
{
	int comps = 0;
	int swaps = 0;
	if (start == stop)
		return;
	I current = start;
	++current;
	while (current != stop)
	{
		I pair = current;
		while (pair != start)
		{
			I prev = pair;
			--prev;
			++comps;
			if (*pair < *prev)
			{
				++swaps;
				swap(*pair, *prev);
			}
			else
				break;
			pair = prev;
		}
		++current;
	}
	//cout << comps << " comparisons, " << swaps << " swaps" << endl;
}


void
insertionSort(vector<double> & sequence)
{
	insertionSortViaIterators(begin(sequence), end(sequence));
}


// post: sequence is sorted from smallest to largest.
void
selectionSort(vector<double> & sequence)
{
	for (size_t first = 0; first < sequence.size() - 1; ++first)
	{
		size_t smallAt = first;
		for (size_t i = first + 1; i < sequence.size(); ++i)
		{
			if (sequence.at(i) < sequence.at(smallAt))
			{
				smallAt = i;
			}
		}
		swap(sequence.at(first), sequence.at(smallAt));
	}
}



// pre:  sequence[start] up to sequence[half] is sorted from smallest to largest.
//       sequence[half] up to sequence[stop] is sorted from smallest to largest.
// post: sequence[start] up to sequence[stop] is sorted from smallest to largest.
void
merge(vector<double> & sequence, size_t start, size_t half, size_t stop)
{
	vector<double> copy(sequence.begin() + start, sequence.begin() + half);
	size_t halfAt = half;
	size_t copyAt = 0;
	size_t sequenceAt = start;
	while (halfAt < stop  &&  copyAt < copy.size())
	{
		if (sequence.at(halfAt) < copy.at(copyAt))
		{
			sequence.at(sequenceAt) = sequence.at(halfAt);
			++sequenceAt;
			++halfAt;
		}
		else
		{
			sequence.at(sequenceAt) = copy.at(copyAt);
			++sequenceAt;
			++copyAt;
		}
	}
	while (halfAt < stop)
	{
		sequence.at(sequenceAt) = sequence.at(halfAt);
		++sequenceAt;
		++halfAt;
	}
	while (copyAt < copy.size())
	{
		sequence.at(sequenceAt) = copy.at(copyAt);
		++sequenceAt;
		++copyAt;
	}
}



// post: sequence[start] up to sequence[stop] is sorted from smallest to largest.
void
mergeSort(vector<double> & sequence, size_t start, size_t stop)
{
	if (stop - start < 2)
	{
		return;
	}

	size_t half = (start + stop) / 2;
	mergeSort(sequence, start, half);
	mergeSort(sequence, half, stop);
	merge(sequence, start, half, stop);
}


// post: sequence is sorted from smallest to largest.
void
mergeSort(vector<double> & sequence)
{
	mergeSort(sequence, 0, sequence.size());
}



// pre:  0 < n
// post: a randomly chosen integer from the uniform distribution
//         0 <= random < n  has been returned.
size_t
randMod(size_t n)
{
	size_t randAt = rand() % n;
	randAt = (randAt * (RAND_MAX + 1) + rand()) % n;
	randAt = (randAt * (RAND_MAX + 1) + rand()) % n;
	randAt = (randAt * (RAND_MAX + 1) + rand()) % n;
	return randAt;
}


// pre:  0 <= start < stop < sequence.size()
// post: the values in sequence[start..stop-1] have been rearranged.
//       pivotAt such that sequence[start..pivotAt-1] 
//         <= sequence[pivotAt] <= sequence[pivotAt+1..stop-1]
//         has been returned.
size_t
partition(vector<double> & sequence, size_t start, size_t stop)
{
	size_t pivotAt = start + randMod(stop - start);
	double pivot = sequence.at(pivotAt);
	sequence.at(pivotAt) = sequence.at(start);
	sequence.at(start) = pivot;

	size_t left = start + 1;
	size_t right = stop - 1;
	while (left <= right)
	{
		while (left < stop && sequence.at(left) < pivot)
			++left;
		while (pivot < sequence.at(right))
			--right;

		if (left <= right)
		{
			swap(sequence.at(left), sequence.at(right));
			++left;
			--right;
		}
	}
	swap(sequence.at(start), sequence.at(right));
	return right;
}


// post: the disorder of the entries in sequence has been returned
int
disorder(vector<double> & sequence)
{
	int dCount = 0;
	for (size_t i1 = 0; i1 < sequence.size(); ++i1)
		for (size_t i2 = i1 + 1; i2 < sequence.size(); ++i2)
			if (sequence.at(i2) < sequence.at(i1))
				++dCount;
	return dCount;
}


// post: sequence is sorted from smallest to largest
void
randSort(vector<double> & sequence)
{
	size_t n = sequence.size();
	while (disorder(sequence) > 0)
	{
		size_t i = randInt(1, n);
		if (sequence.at(i) < sequence.at(i - 1))
			swap(sequence.at(i), sequence.at(i - 1));
	}
}


// post: sequence is sorted from smallest to largest
void
randPlus(vector<double> & sequence)
{
	size_t n = sequence.size();
	size_t n3 = n * n * n;
	for (size_t i = 0; i*i < n3; ++i)
	{
		size_t i1 = randInt(0, n);
		size_t i2 = randInt(0, n);
		if (i2 < i1)
			swap(i1, i2);
		if (sequence.at(i2) < sequence.at(i1))
			swap(sequence.at(i1), sequence.at(i2));
	}
	insertionSort(sequence);
}


// post: sequence[start] up to sequence[stop] is sorted from smallest to largest
//         except for segments smaller than huge.
void
quickSort(vector<double> & sequence, size_t start, size_t stop, size_t huge = 2)
{
	if (stop - start < huge)
	{
		return;
	}

	size_t pivotAt = partition(sequence, start, stop);
	quickSort(sequence, start, pivotAt, huge);
	quickSort(sequence, pivotAt + 1, stop, huge);
}


// post: sequence is sorted from smallest to largest.
void
quickSort(vector<double> & sequence)
{
	quickSort(sequence, 0, sequence.size());
}


// post: sequence is sorted from smallest to largest.
// Note: huge near 10 best for random data
void
quickPlus(vector<double> & sequence)
{
	quickSort(sequence, 0, sequence.size(), 10);
	insertionSort(sequence);
}

void
fastmergeSort(vector<double> & sequence, size_t start, size_t stop)
{
	if (stop - start < 2)
	{
		return;
	}
	if (stop - start < 50)
	{
		vector<double>::iterator istart = sequence.begin() + start;
		vector<double>::iterator iend = sequence.begin() + stop;
		insertionSortViaIterators(istart, iend);
	}
	else
	{
		size_t half = (start + stop) / 2;
		fastmergeSort(sequence, start, half);
		fastmergeSort(sequence, half, stop);
		merge(sequence, start, half, stop);
	}
	
}

void
fastmerge(vector<double> & sequence)
{
	fastmergeSort(sequence, 0, sequence.size());
}


// read one value from cin
// attempt at robust read 
template< typename T >
void
getOne(T & x)
{
	cin >> x;
	while (cin.fail())
	{
		cin.clear();
		cin.ignore(1024, '\n');
		cout << "try again: ";
		cin >> x;
	}
	cin.ignore(1024, '\n');
}


void
runCode(string sortName, void(*sortCode) (vector<double> &),
	string createName, void(*createCode) (vector<double> &, int))
{
	// MIN_TIME is the minimum total time for repeated runs
	// of the sortCode function.
	const double MIN_TIME(2.0);

	// Make one run of sortCode to see if the result is a sorted list
	cout << endl << "Testing " << sortName << " sort function using "
		<< createName << " data." << endl << endl;

	int n = 65;
	while (n > 64)
	{
		cout << "Enter a SMALL non-negative list size: ";
		getOne(n);
	}
	{
		vector<double> sequence;
		(*createCode)(sequence, n);
		cout << "Created list:" << endl;
		write(cout, sequence);
		(*sortCode)(sequence);
		cout << "Sorted list:" << endl;
		write(cout, sequence);
		if (!is_sorted(sequence.begin(), sequence.end()))
			cout << "Sequence is NOT sorted" << endl;
	}

	// Time several runs of sortCode with different list sizes.
	// Each run starts with a freshly created list.
	cout << endl << "Timing " << sortName << " sort function using "
		<< createName << " data." << endl << endl;
	cout << "Enter list size (negative to quit): ";
	getOne(n);

	while (n >= 0)
	{
		Timer watch;
		int runs(0);
		while (watch() < MIN_TIME)
		{
			vector<double> sequence;
			(*createCode)(sequence, n);
			watch.start();
			(*sortCode)(sequence);
			watch.stop();
			++runs;
			if (!is_sorted(sequence.begin(), sequence.end()))
				cout << "Sequence is NOT sorted" << endl;
		}
		cout << runs << " " << createName << " lists of size "
			<< n << " " << sortName << " sorted in " << watch()
			<< " seconds." << endl;
		cout << "   " << (watch() / runs)
			<< " seconds per list." << endl;

		cout << endl;
		cout << "Enter list size (negative to quit): ";
		getOne(n);
	}
}


// Request the sort code name and fill code name from the
//   user.  Set the names and functions for sorting and 
//   filling.
bool
getRequest(string & sortName,
	void(*& sortCode) (vector<double> &),
	string & fillName,
	void(*& fillCode) (vector<double> &, int)
)
{
	while (true)
	{
		cout << endl << "Enter sort (quit to quit): ";
		getOne(sortName);
		if (sortName == "quit")
			return false;
		if (sortName == "slow")
			sortCode = slowSort;
		else if (sortName == "bubble")
			sortCode = bubbleSort;
		else if (sortName == "insertion")
			sortCode = insertionSort;
		else if (sortName == "selection")
			sortCode = selectionSort;
		else if (sortName == "merge")
			sortCode = mergeSort;
		else if (sortName == "quick")
			sortCode = quickSort;
		else if (sortName == "rand")
			sortCode = randSort;
		else if (sortName == "rand++")
			sortCode = randPlus;
		else if (sortName == "quick++")
			sortCode = quickPlus;
		else if (sortName == "fmerge")
			sortCode = fastmerge;
		else
			sortCode = nullptr;

		if (sortCode != nullptr)
		{
			cout << endl << "Enter fill (quit to quit): ";
			getOne(fillName);
			if (fillName == "quit")
				return false;
			else if (fillName == "random")
				fillCode = randomFill;
			else if (fillName == "disordered")
				fillCode = disorderedFill;
			else if (fillName == "ordered")
				fillCode = orderedFill;
			else if (fillName == "reversed")
				fillCode = reversedFill;
			else
				fillCode = nullptr;
		}

		if (sortCode == nullptr || fillCode == nullptr)
		{
			cout << "unknown request" << endl;
			cout << "  known sorts are \"slow\", \"bubble\","
				<< "\"insertion\", \"selection\"," << endl;
			cout << "                  \"merge\", \"fmerge\", \"quick\""
				<< ", \"rand\", \"rand++\", and \"quick++\"" << endl;
			cout << "  known fills are \"random\", \"disordered\""
				<< ", \"ordered\", and \"reversed\"" << endl;
			cout << "or type \"quit\" to quit" << endl;
			cout << endl;
		}
		else
			return true;
	}
}


int
main()
{
	srand(unsigned long(time(nullptr)));

	string sortName;
	void(*sortCode) (vector<double> &);
	string fillName;
	void(*fillCode) (vector<double> &, int);
	while (getRequest(sortName, sortCode, fillName, fillCode))
	{
		runCode(sortName, sortCode, fillName, fillCode);
	}

	return 0;
}