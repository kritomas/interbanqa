#include "database/singleton.hpp"
#include <fstream>
#include "kritase64.hpp"

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

	const std::string SCHEMA = kritase64::decodeToString("YmVnaW4gdHJhbnNhY3Rpb247CgpjcmVhdGUgdGFibGUgQWNjb3VudAooCglpZCBpbnRlZ2VyIHByaW1hcnkga2V5IGNoZWNrKGlkIGJldHdlZW4gOTk5OSBhbmQgOTk5OTkpLAoJYmFsYW5jZSBiaWdpbnQgbm90IG51bGwgZGVmYXVsdCAwIGNoZWNrKGJhbGFuY2UgPj0gMCksCgoJaXNfZHVtbXkgYml0IG5vdCBudWxsIGRlZmF1bHQgMAopOwoKaW5zZXJ0IGludG8gQWNjb3VudCAoaWQsIGlzX2R1bW15KSB2YWx1ZXMgKDk5OTksIDEpOyAtLSBEdW1teSBpbnNlcnQKCmNyZWF0ZSB2aWV3IEJhbGFuY2VfVG90YWwgYXMKCXNlbGVjdCBpZm51bGwoc3VtKGJhbGFuY2UpLCAwKSBmcm9tIEFjY291bnQgd2hlcmUgaXNfZHVtbXkgPSAwOwoKY3JlYXRlIHZpZXcgQWNjb3VudF9Ub3RhbCBhcwoJc2VsZWN0IGNvdW50KCopIGZyb20gQWNjb3VudCB3aGVyZSBpc19kdW1teSA9IDA7Cgpjb21taXQ7");
	sqlite::database db(DB_PATH);
	sqlite3_exec(db.connection().get(), SCHEMA.c_str(), nullptr, nullptr, nullptr);
}

std::shared_ptr<DBSingleton> DBSingleton::_instance;

std::shared_ptr<DBSingleton> DBSingleton::instance()
{
	if (_instance == nullptr) _instance.reset(new DBSingleton);
	return _instance;
}