//made following tutorial at https://www.softwaretestinghelp.com/searching-algorithms-in-cpp/ accessed 17/12/2020

//I then implemented the ability to read in from a file, and declare a dynamic array

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <new>
using namespace std;

int binarySearch(int myarray[], int beg, int end, int key)
{
	int mid;
	if(end >= beg)
	{
		mid = (beg + end) / 2;
		if(myarray[mid] == key)
		{
			return mid + 1;
		}
		else if(myarray[mid] < key)
		{
			return binarySearch(myarray, mid + 1, end, key);
		}
		else
		{
			return binarySearch(myarray, beg, mid - 1, key);
		}
	}
	return -1;
}

int main()
{
	string nameOfFile, line;
	int num = 0;
	int* myArray; //declares a new pointer so that a dynamic array can be created
	cout << "Enter the name of the file to gather the inputs from : "; cin >> nameOfFile;
	nameOfFile += ".txt"; //opens file with the name given
	ifstream in_file(nameOfFile);

	if(!in_file.is_open())
	{
		cout << "Cannot open file." << endl;
		exit(1);
	}
	
	while(getline(in_file, line))
	{
		++num;
	}

	if(num == 0)
	{
		cout << "That file is empty." << endl;
		main();
	}
	
	myArray = new (nothrow) int[num]; //declares array with the size the same amount of the lines in the file.
	
	for(int i = 0; i <= num; i++)
	{
		
	}
	int key, location = -1;
	cout << "The input array is:" << endl;
	for(int i = 0; i < 10; i++)
	{
		cout << myArray[i] << " ";
	}
	cout << endl;
	cout << "Enter the key that is to be searched for : "; cin >> key;
	location = binarySearch(myArray, 0, 9, key);
	if(location != -1)
	{
		cout << "Key found at location " << location;
	}
	else
	{
		cout << "Requested key not found.";
	}

	return 0;
}