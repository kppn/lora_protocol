CPPFLAGS=-std=c++14 -O0
CC=g++

all: protocol.cpp
	$(CC) $(CPPFLAGS) -c protocol.cpp

k: k.cpp
	$(CC) $(CPPFLAGS) -o k k.cpp


test: protocol_test.cpp protocol.cpp
	$(CC) $(CPPFLAGS) -lboost_unit_test_framework -o protocol_test protocol_test.cpp
	./protocol_test

