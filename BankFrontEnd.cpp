#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>
#include "Transaction.cpp"
#include "AccountHolder.cpp"
#include "Account.cpp"

#pragma warning(disable : 4996)

using namespace std;

int main(int argc, char** argv)
{
	bool isAdmin = false;
	bool loop = true;
	int maxAccount = 1;
	AccountHolder *user = &AccountHolder("");
	string input;
	vector<AccountHolder> accountholders;
	vector<Transaction> transactions;

	if (argc != 3)
	{
		cout << "The banking program requires 2 arguments: first is the input file, and the other is the output file.";
		cout << argc;
		exit(1);
	}

	cout << "Welcome to the banking system; please type 'login' to continue: ";

	while (loop)
	{
		cin >> input;

		//log into the banking system
		if (input.compare("login") == 0)
		{
			ifstream inFile;

			inFile.open(argv[1], ios::in);

			if (!inFile.fail())
			{
				string inLine;

				//Read accounts from accounts.txt file
				while (getline(inFile, inLine))
				{
					if (inLine.substr(6, 11).compare("END_OF_FILE") != 0)
					{
						int count = 0;
						while (inLine.at(25 - count) == ' ')
						{
							count++;
						}

						string fullName = inLine.substr(6, 20 - count);
						bool nameFound = false;
						int foundAt;

						//Check if name is already recorded from file
						for (foundAt = 0; foundAt < accountholders.size() && !nameFound; foundAt++)
						{
							if (accountholders.at(foundAt).getFullName().compare(fullName) == 0)
							{
								nameFound = true;
								foundAt--;
							}
						}

						if (nameFound)
						{
							accountholders.at(foundAt).addAccount(Account(stoi(inLine.substr(0, 5)), stod(inLine.substr(29, 8)), inLine[27]));
							if (maxAccount < stoi(inLine.substr(0, 5)))
							{
								maxAccount = stoi(inLine.substr(0, 5));
							}
						}
						else
						{
							accountholders.push_back(AccountHolder(fullName));

							accountholders.at(accountholders.size() - 1).addAccount(Account(stoi(inLine.substr(0, 5)), stod(inLine.substr(29, 8)), inLine[27]));
						}
					}
				}
				maxAccount++;

				inFile.close();

				cout << "Enter a session type, 'standard' or 'admin': ";

				//Determine user type
				while (loop)
				{
					cin >> input;

					if (input.compare("admin") == 0)
					{
						isAdmin = true;

						loop = false;
					}
					else if (input.compare("standard") != 0)
					{
						cout << "Error: '" + input + "' session type is not allowed";
					}
					else
					{
						loop = false;
					}
				}
				loop = true;

				//Get account holder's name if the user is not an admin
				if (!isAdmin)
				{
					cout << "Enter account holder's name: ";
					cin.ignore();

					while (loop)
					{
						getline(cin, input);
						bool nameFound = false;

						for (int i = 0; i < accountholders.size() && !nameFound; i++)
						{
							if (accountholders.at(i).getFullName().compare(input) == 0)
							{
								nameFound = true;
								user = &accountholders.at(i);
							}
						}

						if (nameFound)
						{
							loop = false;
						}
						else
						{
							cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
						}
					}
				}

				loop = true;

				double withdrawLimit = 500;
				double transferLimit = 1000;
				double paymentLimit = 2000;

				cout << "Reading current bank account file...\n";
				cout << "Login Accepted\n";

				while (loop)
				{
					string code = "";
					string name = "";
					int number = 0;
					double amount = 0.0;
					string other = "  ";

					cout << "Please enter a transaction: ";
					cin >> input;

					if (input.compare("login") == 0)
					{
						cout << "Error: You are already logged in\n";
					}
					else if (input.compare("logout") == 0)
					{
						transactions.push_back(Transaction("00", user->getFullName(), 0, 0.0, "  "));

						ofstream outFile(argv[2], ios::app);

						if (!outFile.fail())
						{
							for (int i = 0; i < transactions.size(); i++)
							{
								outFile << transactions.at(i).getTransactionLine() << endl;
							}
						}
						else
						{
							cout << "Something went wrong... please try again later.\n";
						}

						loop = false;

						outFile.close();

						cout << "Logging out...\n";
					}
					else if (input.compare("withdrawal") == 0)
					{
						code = "01";

						AccountHolder *tempUser = &AccountHolder("");

						//Get account holder's name if the user is an admin
						if (isAdmin)
						{
							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;
						}
						else
						{
							tempUser = user;
						}

						name = tempUser->getFullName();

						cout << "Please enter the account number: ";

						Account *account = &Account(0, 0, ' ');

						while (loop)
						{
							cin >> number;

							try
							{
								account = tempUser->getAccount(number);

								loop = false;
							}
							catch (int)
							{
								cout << "The user '" + name + "' does not own that account, please try again: ";
							}
						}
						loop = true;

						cout << "Enter the amount to withdraw: ";

						while (loop)
						{
							cin >> amount;

							if (amount > withdrawLimit && !isAdmin)
							{
								cout << "Error: Amount entered exceeds withdraw limit for the session. Remaining withdraw amount: $" << withdrawLimit << ". Please try again: ";
							}
							else if (amount > account->getFunds())
							{
								cout << "Error: Insufficient funds, funds in account: $" << account->getFunds() << ", please try again: ";
							}
							else
							{
								account->setFunds(account->getFunds() - amount);
								withdrawLimit -= amount;
								loop = false;
							}
						}

						loop = true;

						cout << "Withdrawal successful\n";

						transactions.push_back(Transaction(code, name, number, amount, other));
					}
					else if (input.compare("transfer") == 0)
					{
						code = "02";

						AccountHolder *tempUser = &AccountHolder("");

						//Get account holder's name if the user is an admin
						if (isAdmin)
						{
							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;
						}
						else
						{
							tempUser = user;
						}

						name = tempUser->getFullName();

						string transferName;

						cout << "Please enter the account number that the money will be transfered from: ";

						Account *firstAccount = &Account(0, 0, ' ');

						while (loop)
						{
							cin >> number;

							try
							{
								firstAccount = tempUser->getAccount(number);

								loop = false;
							}
							catch (int)
							{
								cout << "The user '" + name + "' does not own that account, please try again: ";
							}
						}
						loop = true;

						cout << "Please enter the account number that the money will be transfered to: ";

						Account *secondAccount = &Account(0, 0, ' ');

						while (loop)
						{
							cin >> number;

							for (int i = 0; i < accountholders.size() && loop; i++)
							{
								try
								{
									secondAccount = accountholders.at(i).getAccount(number);
									transferName = accountholders.at(i).getFullName();
									loop = false;
								}
								catch (int)
								{

								}
							}
							if (loop)
							{
								cout << "Account number not found, please try again: ";
							}
						}
						loop = true;

						cout << "Enter the amount to transfer: ";

						while (loop)
						{
							cin >> amount;

							if (amount > transferLimit && !isAdmin)
							{
								cout << "Error: Amount entered exceeds transfer limit for the session. Remaining transfer amount: $" << transferLimit << ". Please try again: ";
							}
							else if (amount > firstAccount->getFunds())
							{
								cout << "Error: Insufficient funds, funds in account: $" << firstAccount->getFunds() << ", please try again: ";
							}
							else
							{
								firstAccount->setFunds(firstAccount->getFunds() - amount);
								secondAccount->setFunds(secondAccount->getFunds() + amount);
								transferLimit -= amount;
								loop = false;
							}
						}

						loop = true;

						cout << "Transfer successful\n";

						transactions.push_back(Transaction(code, name, firstAccount->getAccountNumber(), amount, other));
						transactions.push_back(Transaction(code, transferName, secondAccount->getAccountNumber(), amount, other));
					}
					else if (input.compare("paybill") == 0)
					{
						code = "03";

						AccountHolder *tempUser = &AccountHolder("");

						//Get account holder's name if the user is an admin
						if (isAdmin)
						{
							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;
						}
						else
						{
							tempUser = user;
						}

						name = tempUser->getFullName();

						cout << "Please enter the account number: ";

						Account *account = &Account(0, 0, ' ');

						while (loop)
						{
							cin >> number;

							try
							{
								account = tempUser->getAccount(number);

								loop = false;
							}
							catch (int)
							{
								cout << "The user '" + name + "' does not own that account, please try again: ";
							}
						}
						loop = true;

						cout << "Enter the amount to pay: ";

						while (loop)
						{
							cin >> amount;

							if (amount > paymentLimit && !isAdmin)
							{
								cout << "Error: Amount entered exceeds payment limit for the session. Remaining payment amount: $" << paymentLimit << ". Please try again: ";
							}
							else if (amount > account->getFunds())
							{
								cout << "Error: Insufficient funds, funds in account: $" << account->getFunds() << ", please try again: ";
							}
							else
							{
								account->setFunds(account->getFunds() - amount);
								paymentLimit -= amount;
								loop = false;
							}
						}

						loop = true;

						cout << "Enter the payee: ";

						while (loop)
						{
							cin >> input;

							if (input.compare("EC") == 0 || input.compare("CQ") == 0 || input.compare("FI") == 0)
							{
								other = input;
								loop = false;
							}
							else
							{
								cout << "Please enter one of the following companies (EC, CQ, or FI): ";
							}
						}
						loop = true;

						cout << "Payment successful\n";

						transactions.push_back(Transaction(code, name, number, amount, other));
					}
					else if (input.compare("deposit") == 0)
					{
						code = "04";

						AccountHolder *tempUser = &AccountHolder("");

						//Get account holder's name if the user is an admin
						if (isAdmin)
						{
							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;
						}
						else
						{
							tempUser = user;
						}

						name = tempUser->getFullName();

						cout << "Please enter the account number: ";

						Account *account = &Account(0, 0, ' ');

						while (loop)
						{
							cin >> number;

							try
							{
								account = tempUser->getAccount(number);

								loop = false;
							}
							catch (int)
							{
								cout << "The user '" + name + "' does not own that account, please try again: ";
							}
						}
						loop = true;

						cout << "Enter the amount to deposit: ";

						while (loop)
						{
							cin >> amount;

							if (amount > 100000 - account->getFunds())
							{
								cout << "Error: Amount exceeds the account max of $100,000, funds in account: $" << account->getFunds() << ", please try again: ";
							}
							else
							{
								account->setFunds(account->getFunds() + amount);
								loop = false;
							}
						}

						loop = true;

						cout << "Deposit successful\n";

						transactions.push_back(Transaction(code, name, number, amount, other));
					}
					else if (input.compare("create") == 0)
					{
						code = "05";

						//Only run transaction if user is an admin
						if (isAdmin)
						{
							cout << "Enter account holder's name: ";
							cin.ignore();

							getline(cin, name);

							number = maxAccount + 1;
							maxAccount++;

							cout << "Enter the amount to deposit in new account: ";

							while (loop)
							{
								cin >> amount;

								if (amount > 100000)
								{
									cout << "Error: Amount exceeds the account max of $100,000, please try again: ";
								}
								else
								{
									loop = false;
								}
							}

							loop = true;

							cout << "Creation successful\n";

							transactions.push_back(Transaction(code, name, number, amount, other));
						}
						else
						{
							cout << "Error: not logged in as admin\n";
						}
					}
					else if (input.compare("delete") == 0)
					{
						code = "06";

						//Only run transaction if user is an admin
						if (isAdmin)
						{
							AccountHolder *tempUser = NULL;

							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;

							name = tempUser->getFullName();

							cout << "Please enter the account number: ";

							Account *account = &Account(0, 0, ' ');

							while (loop)
							{
								cin >> number;

								try
								{
									account = tempUser->getAccount(number);

									loop = false;
								}
								catch (int)
								{
									cout << "The user '" + name + "' does not own that account, please try again: ";
								}
							}
							loop = true;

							if (account->getFunds() > 0)
							{
								cout << "Error: To delete this account, all $" << account->getFunds() << " must be withdrawn\n";
							}
							else
							{
								cout << "Deletion successful\n";

								if (tempUser->removeAccount(number) == 0)
								{
									bool removed = false;
									for (int i = 0; i < accountholders.size() && !removed; i++)
									{
										if (accountholders.at(i).getFullName().compare(name) == 0)
										{
											accountholders.erase(accountholders.begin() + i);
										}
									}
								}

								transactions.push_back(Transaction(code, name, number, amount, other));
							}
						}
						else
						{
							cout << "Error: not logged in as admin\n";
						}
					}
					else if (input.compare("disable") == 0)
					{
						code = "07";

						//Only run transaction if user is an admin
						if (isAdmin)
						{
							AccountHolder *tempUser = NULL;

							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;

							name = tempUser->getFullName();

							cout << "Please enter the account number: ";

							Account *account = &Account(0, 0, ' ');

							while (loop)
							{
								cin >> number;

								try
								{
									account = tempUser->getAccount(number);

									loop = false;
								}
								catch (int)
								{
									cout << "The user '" + name + "' does not own that account, please try again: ";
								}
							}
							loop = true;

							cout << "Account Disabled\n";

							transactions.push_back(Transaction(code, name, number, amount, other));
						}
						else
						{
							cout << "Error: not logged in as admin\n";
						}
					}
					else if (input.compare("changeplan") == 0)
					{
						code = "08";

						//Only run transaction if user is an admin
						if (isAdmin)
						{
							AccountHolder *tempUser = NULL;

							cout << "Enter account holder's name: ";
							cin.ignore();

							while (loop)
							{
								getline(cin, input);
								bool nameFound = false;

								for (int i = 0; i < accountholders.size() && !nameFound; i++)
								{
									if (accountholders.at(i).getFullName().compare(input) == 0)
									{
										nameFound = true;
										tempUser = &accountholders.at(i);
									}
								}

								if (nameFound)
								{
									loop = false;
								}
								else
								{
									cout << "Error: account name '" + input + "' not found (Please input name correctly): ";
								}
							}

							loop = true;

							name = tempUser->getFullName();

							cout << "Please enter the account number: ";

							Account *account = &Account(0, 0, ' ');

							while (loop)
							{
								cin >> number;

								try
								{
									account = tempUser->getAccount(number);

									loop = false;
								}
								catch (int)
								{
									cout << "The user '" + name + "' does not own that account, please try again: ";
								}
							}
							loop = true;

							cout << "Enter the payment plan: ";

							while (loop)
							{
								cin >> input;

								if (input.compare("SP") == 0 || input.compare("NP") == 0)
								{
									other = input;
									loop = false;
								}
								else
								{
									cout << "Please enter one of the following plans (SP or NP): ";
								}
							}
							loop = true;

							cout << "Plan Changed\n";

							transactions.push_back(Transaction(code, name, number, amount, other));
						}
						else
						{
							cout << "Error: not logged in as admin\n";
						}
					}
					else
					{
						cout << input + " is not a valid transaction\n";
						cout << "Please enter a transaction: ";
					}
				}
			}
			else
			{
				cout << "Something went wrong; please try again later.\n";
			}

			isAdmin = false;
			loop = true;
			cout << "Welcome to the banking system; please type 'login' to continue: ";
		}
		//exit the program
		else if (input.compare("exit") == 0)
		{
			loop = false;
			cout << "Thank you for using the banking system";
		}
		//handle invalid inputs
		else
		{
			cout << "Error: cannot perform transactions until you are logged in; please type 'login' to continue: ";
		}
	}

    return 0;
}
