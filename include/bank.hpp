#ifndef BANK_HPP
#define BANK_HPP

class Bank
{
public:
	long long int balance = 0;
	int clients = 0;

	double balancePerClient() const;

	static void listBanks();
};

#endif