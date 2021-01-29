//made following the tutorial at https://www.softwaretestinghelp.com/searching-algorithms-in-cpp/ accessed 17/12/2020

#include <iostream>
using namespace std;
int main()
{
	int myArray[10] = { 21,43,23,54,75,13,5,8,25,10 };
	int key, loc;
	cout << "The input array is:" << endl;
	for(int i = 0;i<10;i++)
	{
		cout << myArray[i] << " "; //outputs the entire array
	}

	cout << endl;
	cout << "Enter the key to be searched : "; cin >> key; //gets the item to be searched for

	for(int i = 0; i<10; i++)
	{
		if (myArray[i] == key) //if that index = the key, store the location and exit the loop
		{
			loc = i + 1;
			break;
		}
		else loc = 0; //if not found, location = 0
	}

	if(loc != 0) //checks if the key was found
	{
		cout << "Key found at position " << loc << " in the array."; //outputs where the key was found
	}
	else
	{
		cout << "Could not find the given key in the array."; //else outputs that the key wasn't found
	}

	return 0;
}