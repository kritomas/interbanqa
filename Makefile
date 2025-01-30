SRC = src/*.cpp src/*/*.cpp

all : $(SRC)
	gcc -c external/sqlite-amalgamation/sqlite3.c -o sqlite3.o
	g++ $(SRC) -Iinclude -Iexternal/nlohmann -Iexternal/sqlite_modern_cpp/hdr -Iexternal/sqlite-amalgamation sqlite3.o -Wall -O0 -g -fsanitize=undefined -o interbanqa