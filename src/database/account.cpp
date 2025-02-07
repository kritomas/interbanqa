#include "database/account.hpp"
#include "database/singleton.hpp"
#include "exception.hpp"

const int MIN_NUMBER = 10000, MAX_NUMBER = 99999;

void Account::checkNumber(int number)
{
	if (number < MIN_NUMBER || number > MAX_NUMBER)
	{
		throw InterbanqaException("Account number out of range");
	}
}
bool Account::has(int number)
{
	auto singleton = DBSingleton::instance();
	int count;
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
	*singleton->db << "select count(*) from Account where id = ?" << number >> count;
	return count > 0;
}

Account::Account()
{
}

Account Account::create()
{
	auto singleton = DBSingleton::instance();
	if (true)
	{
		std::lock_guard<std::mutex> lock(singleton->db_mutex);
		*singleton->db << "insert into Account default values;";
	}
	return get(singleton->db->last_insert_rowid());
}
void Account::remove(int number)
{
	checkNumber(number);
	auto singleton = DBSingleton::instance();
	Account ac = get(number);
	if (ac._balance > 0)
	{
		throw InterbanqaException("Cannot remove account with value");
	}
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
	*singleton->db << "delete from Account where id = ?;" << number;
}
Account Account::get(int number)
{
	checkNumber(number);
	if (!has(number))
	{
		throw InterbanqaException("Account doesn't exist");
	}
	auto singleton = DBSingleton::instance();
	Account res;
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
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
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
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
		throw InterbanqaException("Amount must not be negative");
	}
	_balance += amount;
	if (_balance < 0)
	{
		throw InterbanqaException("Cannot deposit that much");
	}
	save();
}
void Account::withdraw(long long int amount)
{
	if (amount < 0)
	{
		throw InterbanqaException("Amount must not be negative");
	}
	_balance -= amount;
	if (_balance < 0)
	{
		throw InterbanqaException("Cannot withdraw that much");
	}
	save();
}

long long int Account::count()
{
	auto singleton = DBSingleton::instance();
	long long int res;
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
	*singleton->db << "select * from Account_Total" >> res;
	return res;
}
long long int Account::funds()
{
	auto singleton = DBSingleton::instance();
	long long int res;
	std::lock_guard<std::mutex> lock(singleton->db_mutex);
	*singleton->db << "select * from Balance_Total" >> res;
	return res;
}