all:
	g++ -march=corei7 -O3 -std=c++11 -lboost -Wall -fexceptions -Iinclude -I/usr/include -c /home/dvs23/projects/WordCount/main.cpp -o obj/Debug/main.o
	g++ -march=corei7 -O3 -std=c++11 -lboost -Wall -fexceptions -Iinclude -I/usr/include -c /home/dvs23/projects/WordCount/src/WordCount.cpp -o obj/Debug/src/WordCount.o
	g++ -march=corei7 -O3 -std=c++11 -lboost -Wall -fexceptions -Iinclude -I/usr/include -c /home/dvs23/projects/WordCount/src/Wort.cpp -o obj/Debug/src/Wort.o
	g++  -o bin/Debug/WordCount obj/Debug/main.o obj/Debug/src/WordCount.o obj/Debug/src/Wort.o  -lmysqlcppconn -pthread -Wl,--no-as-needed -ldl  /usr/lib/libboost_regex.a
