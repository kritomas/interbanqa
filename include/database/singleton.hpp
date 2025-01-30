#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>
#include "sqlite_modern_cpp.h"

class DBSingleton
{
private:
	DBSingleton();

	void reset();

	static std::shared_ptr<DBSingleton> _instance;

public:
	std::unique_ptr<sqlite::database> db;

	static std::shared_ptr<DBSingleton> instance();
};

#endif