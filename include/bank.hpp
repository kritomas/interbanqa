#ifndef BANK_HPP
#define BANK_HPP

#include <set>
#include <string>

class Bank
{
public:
	std::string address = "0.0.0.0";
	long long int balance = 0;
	int clients = 0;

	double balancePerClient() const;

	bool operator<(const Bank& other) const;

	static std::multiset<Bank> listBanks();
};

#endif