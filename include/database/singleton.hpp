#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include <mutex>
#include "sqlite_modern_cpp.h"

class DBSingleton
{
private:
	DBSingleton();

	void reset();

	static std::shared_ptr<DBSingleton> _instance;

public:
	std::unique_ptr<sqlite::database> db;
	std::mutex db_mutex;

	static std::shared_ptr<DBSingleton> instance();
};

#endif