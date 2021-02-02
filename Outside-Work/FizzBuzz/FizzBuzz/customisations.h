#pragma once
#define SLEEPVALUE 2500      //Set time for the sleep function so that it can be changed from one place
#define MAXFACTORAMOUNT 10   //Same as above but with the maximum amount of factors allowed
#define LOOPSLEEPTIME 500    //Sets a half second pause between each output of the main loop
#include <iostream>
#include <Windows.h>  //Included for the sleep function
#include <cstdlib>   //Included for the system("cls") command
using namespace std;

//Header file containing all of the functions to alter the fizzbuzz program as necessary

void FizzBuzz();                            //Runs the other functions and outputs the fizzbuzz statements
void GetNumberOfFactors(int* factorAmount); //Gets the amount of factors to check against
void GetRange(int* min, int* max);          //Gets the range to operate within e.g 1-250


inline void FizzBuzz()
{
	int numOfFactors, currentFactor;
	int rangeMin, rangeMax, numberOfFactors;
	string currentPhrase, currentOutput;
	bool repeatedValue = false;

	GetRange(&rangeMin, &rangeMax);      //Calls the GetRange function to get the min and max value from the user
	GetNumberOfFactors(&numOfFactors);   //Gets the amount of factors

	int* valueOfFactors = new int[numOfFactors]; //Declares a dynamic array based on the amount of factors requested by the user
	string* phrases = new string[numOfFactors];  //Declares a dynamic array which will store the selected phrases

	//Initialise the array and then populate it
	for (int i = 0; i <= (numOfFactors - 1); i++)
	{
		valueOfFactors[i] = 0;
	}

	for(int i = 0; i <= (numOfFactors - 1); i++)
	{
		do
		{
			repeatedValue = false;
			cout << "Enter a factor: ";
			cin >> currentFactor;
			
			for(int j = 0; j <= (numOfFactors - 1); j++)
			{
				if(valueOfFactors[j] == currentFactor)
				{
					cout << "That factor has already been entered, please choose another.";
					repeatedValue = true;
					break;
				}
			}
		} while (repeatedValue);
		
		valueOfFactors[i] = currentFactor;
		system("cls");
	}
	system("cls");

	//Gets phrases
	for (int i = 0; i <= (numOfFactors - 1); i++)
	{
		do
		{
			repeatedValue = false;
			cout << "Enter a phrase: ";
			cin >> currentPhrase;

			for (int j = 0; j <= (numOfFactors - 1); j++)
			{
				if (phrases[j] == currentPhrase)
				{
					cout << "That phrase has already been entered, please choose another.";
					repeatedValue = true;
					break;
				}
			}
		} while (repeatedValue);

		phrases[i] = currentPhrase;
		system("cls");
	}
	system("cls");
	
	//MAIN FIZZBUZZ LOOP
	for(int i = rangeMin; i <= rangeMax; i++)
	{
		Sleep(LOOPSLEEPTIME);
		currentOutput = "";
		for(int j = 0; j <= (numOfFactors - 1); j++)
		{
			if(i % valueOfFactors[j] == 0)
			{
				currentOutput += phrases[j];
			}
		}
		if(currentOutput.empty())
		{
			cout << i << endl;
		}
		else
		{
			cout << currentOutput << endl;
		}
	}
}

inline void GetRange(int* min, int* max)
{
	*min = 0;
	*max = 0;

	while ((*min == *max || *max < *min) || (*max == 0) || (*min < 0))                    //Loops while start and end are the same or end is less than start or if max value is 0 or min is less than 0
	{
		cout << "Enter the starting point for the loop: ";
		cin >> *min;                                       //Gets the user to input the starting value of the loop
		cout << endl;

		cout << "Enter the end point for the loop: ";
		cin >> *max;                                       //Gets the user to input the end value of the loop
		cout << endl;

		if (*min == *max)
		{
			system("cls");
			cout << "Please enter two different numbers." << endl;
			Sleep(SLEEPVALUE);  //Waits for 1.5 seconds to allow the user to read the message
			system("cls");    //Clears the screen
		}
		else if (*max < *min)
		{
			system("cls");
			cout << "Please enter a maximum higher than the minimum.";
			Sleep(SLEEPVALUE);
			system("cls");
		}
		else if (*min < 0)
		{
			system("cls");
			cout << "Please enter a minimum value of 0 or above.";
			Sleep(SLEEPVALUE);
			system("cls");
		}
		else if (*max == 0)
		{
			system("cls");
			cout << "Please enter a maximum value of more than 0";
			Sleep(SLEEPVALUE);
			system("cls");
		}
	}
	system("cls");
}

inline void GetNumberOfFactors(int* factorAmount)
{
	*factorAmount = 0;
	while(*factorAmount <= 0 || *factorAmount > MAXFACTORAMOUNT)
	{
		*factorAmount = 0;
	    cout << "Enter the amount of factors you wish to use: ";
		cin >> *factorAmount;
		system("cls");

		if(*factorAmount <= 0)
		{
			cout << "Please enter a value more than 0.";
			Sleep(SLEEPVALUE);
			system("cls");
		}
		else if(*factorAmount > MAXFACTORAMOUNT)
		{
			cout << "Please enter a value less than 10.";
			Sleep(SLEEPVALUE);
			system("cls");
		}
	}
	system("cls");
}
