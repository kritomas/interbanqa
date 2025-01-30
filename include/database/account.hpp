#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

class Account
{
private:
	int _number;
	long long int _balance;

	static void checkNumber(int number);
	static bool has(int number);

	Account();

public:
	static Account create();
	static void remove(int number);
	static Account get(int number);
	void save();

	int number();
	long long int balance();
	void deposit(long long int amount);
	void withdraw(long long int amount);

	static long long int count();
	static long long int funds();
};

#endif