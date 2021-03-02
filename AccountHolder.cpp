#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "Account.cpp"

using namespace std;

#ifndef ACCOUNT_HOLDER
#define ACCOUNT_HOLDER

class AccountHolder
{
public:
	//Constructor
	AccountHolder(string name)
	{
		setFullName(name);
	}

	//Returns the account that matches the number, or throw an exception if it's not found
	Account *getAccount(int accountNumber)
	{
		for (int i = 0; i < accounts.size(); i++)
		{
			if (accounts.at(i).getAccountNumber() == accountNumber)
			{
				return &accounts.at(i);
			}
		}

		throw 0;//(exception("Account does not own account #" + accountNumber));
	}

	//Add an account
	void addAccount(Account account)
	{
		accounts.push_back(account);
	}

	//Remove an account and returns the number of accounts left
	int removeAccount(int number)
	{
		bool removed = false;
		for (int i = 0; i < accounts.size() && !removed; i++)
		{
			if (accounts.at(i).getAccountNumber() == number)
			{
				accounts.erase(accounts.begin() + i);
			}
		}

		return accounts.size();
	}

	//Returns the full name
	string getFullName()
	{
		return fullName;
	}

	//Sets the full name
	void setFullName(string newName)
	{
		fullName = newName;
	}

private:
	string fullName;
	vector<Account> accounts;
};

#endif