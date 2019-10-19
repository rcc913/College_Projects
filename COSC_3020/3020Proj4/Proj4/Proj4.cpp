#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include "winTimer.h"
using namespace std;

#define storage vector<vector<node>>

struct node {
	pair<int, int> previous;
	int value;
};

storage LevenshteinDistance(string s, string t)
{
	s = " " + s;
	t = " " + t;
	int m = s.size();
	int n = t.size();

	storage myStorage(m, vector<node> (n));
	for (int i = 0; i < s.size(); i++)
	{
		myStorage[i][0].value = i;
		myStorage[i][0].previous = make_pair(i, 0);
	}

	for (int i = 1; i < m; i++)
	{
		for (int j = 1; j < n; j++)
		{
			int cost;
			if (s[i] == t[j])
			{
				cost = 0;
			}
			else
			{
				cost = 1;
			}
			int temp1 = myStorage[i - 1][j].value + 1;
			int temp2 = myStorage[i][j - 1].value + 1;
			int temp3 = myStorage[i - 1][ j - 1].value + cost;
			if (temp1 <= temp2 && temp1 <= temp3)
			{
				myStorage[i][j].value = temp1;
				myStorage[i][j].previous = make_pair(i - 1, j);
			}
			else if (temp2 <= temp1 && temp2 <= temp3)
			{
				myStorage[i][j].value = temp2;
				myStorage[i][j].previous = make_pair(i, j-1);
			}
			else
			{
				myStorage[i][j].value = temp3;
				myStorage[i][j].previous = make_pair(i - 1, j-1);
			}
		}
	}
	return myStorage;
}


int main()
{
	ifstream inFile;
	inFile.open("strings2.txt");
	string text;
	string temp;
	getline(inFile, text);
	temp = text;
	string pattern;
	cout << "Enter a pattern to find:" << endl;
	getline(cin, pattern);
	int errorsAllowed;
	cout << "Enter amount of acceptable errors:" << endl;
	cin >> errorsAllowed;
	//while (true)
	//{
		//text = temp;
		//cout << "Enter exponential mulitplier of 1000 for size adjustment (larger than 0)" << endl;
		//int multiplier = 0;
		//cin >> multiplier;
		//while (multiplier <= 0)
		//{
			//cout << "Enter mulitplier of 1000 for size adjustment (larger than 0)" << endl;
			//cin >> multiplier;
		//}
		//for (int k = 1; k < multiplier; k++)
		//{
			//text = text + text;
		//}
		Timer t;
		t.start();
		storage s = LevenshteinDistance(pattern, text);
		t.stop();
		int start = text.size() + 1;
		int colStart = 0;
		for (int i = 0; i <= text.size(); i++)
		{
			int temp = s[pattern.size()][i].value;
			if (temp <= start)
			{
				start = temp;
				colStart = i;
			}
		}
		bool check = true;
		int oldRow = pattern.size();
		int oldColumn = colStart;
		int row = 0;
		int column=0;
		string match = "";
		if (start > errorsAllowed)
		{
			cout << "No matching pattern found with less than " << errorsAllowed << " errors" << endl;
		}
		else
		{
			while (check)
			{
				if (oldRow == 0)
				{
					check = false;
				}
				else
				{
					row = s[oldRow][oldColumn].previous.first;
					column = s[oldRow][oldColumn].previous.second;
					if (oldColumn != column)
					{
						match = text[oldColumn - 1] + match;
					}
					oldColumn = column;
					oldRow = row;
				}
			}
			cout << "Match found at position " << oldColumn << " the string is: " << match << endl;
		}
		cout << "For size: " << pattern.size() << " by " << text.size() << " found in: " << t() << endl;
	//}
	inFile.close();
	return 0;
}