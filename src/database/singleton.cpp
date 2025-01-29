#include "database/singleton.hpp"
#include <fstream>

const char* DB_PATH = "interbanqa.db";

DBSingleton::DBSingleton()
{
	std::fstream file(DB_PATH, std::ios::in);
	bool open = file.is_open();
	file.close();
	if (!open)
	{
		reset();
	}

	db = std::make_unique<sqlite::database>(DB_PATH);
}

void DBSingleton::reset()
{
	std::fstream file(DB_PATH, std::ios::out | std::ios::trunc | std::ios::binary);
	file.close();

	sqlite::database db(DB_PATH);
	// TODO: Init
}

std::shared_ptr<DBSingleton> DBSingleton::instance()
{
	if (_instance == nullptr) _instance.reset(new DBSingleton);
	return _instance;
}