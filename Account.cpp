#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

#ifndef ACCOUNT
#define ACCOUNT

class Account
{
public:
	//Constructor
	Account(int number, double funds, char stat)
	{
		setAccountNumber(number);
		setFunds(funds);
		setAccountStatus(stat);
	}

	//Returns the account number
	int getAccountNumber()
	{
		return accountNumber;
	}

	//Sets the account number
	void setAccountNumber(int newNumber)
	{
		accountNumber = newNumber;
	}

	//Returns the amount of funds
	double getFunds()
	{
		return funds;
	}

	//Sets the amount of funds
	void setFunds(double newFunds)
	{
		funds = newFunds;
	}

	//Returns the account status
	char getAccountStatus()
	{
		return status;
	}

	//Sets the account status
	void setAccountStatus(char newStatus)
	{
		status = newStatus;
	}

private:
	int accountNumber;
	double funds;
	char status;
	string accountType;
};

#endif