#include "bank.hpp"

double Bank::balancePerClient() const
{
	if (clients == 0)
	{
		return 0;
	}
	return balance / (double)clients;
}

void Bank::listBanks()
{
	
}