#include "database/singleton.hpp"
#include <fstream>

const char* DB_PATH = "interbanqa.db";

DBSingleton::DBSingleton()
{
	std::ifstream file(DB_PATH);
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

	std::ifstream schemaIO("database/init.sql", std::ios::ate);
	int size = schemaIO.tellg();
	schemaIO.close();
	std::string schema;
	schema.resize(size);
	schemaIO.open("database/init.sql");
	schemaIO.read(schema.data(), size);

	sqlite::database db(DB_PATH);
	sqlite3_exec(db.connection().get(), schema.c_str(), nullptr, nullptr, nullptr);
}

std::shared_ptr<DBSingleton> DBSingleton::_instance;

std::shared_ptr<DBSingleton> DBSingleton::instance()
{
	if (_instance == nullptr) _instance.reset(new DBSingleton);
	return _instance;
}