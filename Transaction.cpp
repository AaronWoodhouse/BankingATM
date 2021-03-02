#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

#ifndef TRANSACTION
#define TRANSACTION

class Transaction
{
	public:
		//Constructor
		Transaction(string code, string name, int number, double amount, string other)
		{
			setTransactionCode(code);
			setAccountName(name);
			setAccountNumber(number);
			setTransactionAmount(amount);
			setOther(other);
		}

		//Returns the transaction in the format required for the transaction file
		string getTransactionLine()
		{
			string returnString = "";

			returnString += transactionCode + " " + accountName;

			while (returnString.length() < 24)
			{
				returnString += " ";
			}

			returnString += accountNumber + " ";

			stringstream ss;
			ss << setw(8) << setfill('0') << setprecision(2) << fixed << transactionAmount;

			returnString += ss.str() + " " + otherInfo;

			return returnString;
		}

		//Returns the transaction code
		string getTransactionCode()
		{
			return transactionCode;
		}

		//Sets the transaction code
		void setTransactionCode(string newCode)
		{
			transactionCode = newCode;
		}

		//Returns the account name
		string getAccountName()
		{
			return accountName;
		}

		//Sets the account name
		void setAccountName(string newName)
		{
			accountName = newName;
		}

		//Returns the account number
		string getAccountNumber()
		{
			return accountNumber;
		}

		//Sets the account number
		void setAccountNumber(int newNumber)
		{
			stringstream ss;
			ss << setw(5) << setfill('0') << newNumber;

			accountNumber = ss.str();
		}

		//Returns the transaction amount
		double getTransactionAmount()
		{
			return transactionAmount;
		}

		//Sets the transaction amount
		void setTransactionAmount(double newAmount)
		{
			transactionAmount = newAmount;
		}

		//Returns the other information
		string getOther()
		{
			return otherInfo;
		}

		//Sets the other information
		void setOther(string newOther)
		{
			otherInfo = newOther;
		}

	private:
		string transactionCode;
		string accountName;
		string accountNumber;
		double transactionAmount;
		string otherInfo;
};

#endif