#include "database/account.hpp"
#include "database/singleton.hpp"
#include <stdexcept>

const int MIN_NUMBER = 10000, MAX_NUMBER = 99999;

void Account::checkNumber(int number)
{
	if (number < MIN_NUMBER || number > MAX_NUMBER)
	{
		throw std::runtime_error("Account number out of range");
	}
}

Account Account::create()
{
	auto singleton = DBSingleton::instance();
	*singleton->db << "insert into Account default values;";
	return get(singleton->db->last_insert_rowid());
}
void Account::remove(int number)
{
	checkNumber(number);
	auto singleton = DBSingleton::instance();
	Account ac = get(number);
	if (ac._balance > 0)
	{
		throw std::runtime_error("Cannot remove account with value");
	}
	*singleton->db << "delete from Account where id = ?;" << number;
}
Account Account::get(int number)
{
	checkNumber(number);
	auto singleton = DBSingleton::instance();
	Account res;
	*singleton->db << "select id, balance from Account where id = ?" << number >> [&](int number, long long int balance)
	{
		res._number = number;
		res._balance = balance;
	};
	return res;
}
void Account::save()
{
	auto singleton = DBSingleton::instance();
	*singleton->db << "update Account set balance = ? where id = ?" << _balance << _number;
}

int Account::number()
{
	return _number;
}
long long int Account::balance()
{
	return _balance;
}

void Account::deposit(long long int amount)
{
	if (amount < 0)
	{
		throw std::runtime_error("Amount must not be negative");
	}
	_balance += amount;
	if (_balance < 0)
	{
		throw std::runtime_error("Cannot deposit that much");
	}
	save();
}
void Account::withdraw(long long int amount)
{
	if (amount < 0)
	{
		throw std::runtime_error("Amount must not be negative");
	}
	_balance -= amount;
	if (_balance < 0)
	{
		throw std::runtime_error("Cannot withdraw that much");
	}
	save();
}