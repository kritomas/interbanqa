AUTOMAKE_OPTIONS = foreign subdir-objects
bin_PROGRAMS = interbanqa
SUBIDRS = src

interbanqa_SOURCES = ./src/bank.cpp \
./src/client.cpp \
./src/config.cpp \
./src/exception.cpp \
./src/kritase64.cpp \
./src/log.cpp \
./src/main.cpp \
./src/server.cpp \
./src/stringops.cpp \
./src/database/account.cpp \
./src/database/singleton.cpp \
./src/networking/acceptor.cpp \
./src/networking/connection.cpp \
./src/networking/packet.cpp \
./src/networking/socket.cpp \
./external/sqlite-amalgamation/sqlite3.c

dist_interbanqa_SOURCES = ./include/bank.hpp \
./include/client.hpp \
./include/config.hpp \
./include/exception.hpp \
./include/kritase64.hpp \
./include/log.hpp \
./include/server.hpp \
./include/stringops.hpp \
./include/database/account.hpp \
./include/database/singleton.hpp \
./include/networking/acceptor.hpp \
./include/networking/connection.hpp \
./include/networking/packet.hpp \
./include/networking/socket.hpp \
./external/nlohmann/json.hpp \
./external/sqlite-amalgamation/sqlite3.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/errors.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/log.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/sqlcipher.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/lists/error_codes.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/utility/function_traits.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/utility/uncaught_exceptions.h \
./external/sqlite_modern_cpp/hdr/sqlite_modern_cpp/utility/variant.h

AM_CPPFLAGS = -I./include -I./external/sqlite-amalgamation -I./external/nlohmann -I./external/sqlite_modern_cpp/hdr

if WINDOWS
  LIBS += -lws2_32
endif

dist_doc_DATA = README.md sources.md