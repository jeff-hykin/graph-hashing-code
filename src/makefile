# The graph hash makefile.

CC = gcc

#CCFLAGS = -O3 -c
#LINKLIBS = -lm -lstdc++
CCFLAGS = -DTHREADS -O3 -c
LINKLIBS = -lpthread -lm -lstdc++

all: ../bin/graph_hash ../bin/graph_isomorph_tester ../bin/graph_hash_special_tester

../bin/graph_hash: graphHashDriver.o graphHash.o graph.o md5.o random.o fileio.o gettime.o
	$(CC) -o ../bin/graph_hash graphHashDriver.o graphHash.o graph.o md5.o random.o fileio.o gettime.o $(LINKLIBS)

../bin/graph_isomorph_tester: graphIsomorphTester.o graphHash.o graph.o graphIsomorph.o md5.o random.o fileio.o gettime.o
	$(CC) -o ../bin/graph_isomorph_tester graphIsomorphTester.o graphHash.o graph.o graphIsomorph.o md5.o random.o fileio.o gettime.o $(LINKLIBS)

../bin/graph_hash_special_tester: graphHashSpecialTester.o graphHashBug1Tester.o graphHashBug2Tester.o graphHashRegularGraphTester.o graphHashCaseTester.o graph_ge.o graphHash.o graph.o graphIsomorph.o md5.o random.o fileio.o gettime.o
	$(CC) -o ../bin/graph_hash_special_tester graphHashSpecialTester.o graphHashBug1Tester.o graphHashBug2Tester.o graphHashRegularGraphTester.o graphHashCaseTester.o graph_ge.o graphHash.o graph.o graphIsomorph.o md5.o random.o fileio.o gettime.o $(LINKLIBS)

graphHashDriver.o: graphHashDriver.cpp
	$(CC) $(CCFLAGS) graphHashDriver.cpp

graphIsomorphTester.o: graphIsomorphTester.cpp
	$(CC) $(CCFLAGS) graphIsomorphTester.cpp

graphHash.o: graphHash.hpp graphHash.cpp graph.hpp
	$(CC) $(CCFLAGS) graphHash.cpp

graph.o: graph.hpp graph.cpp
	$(CC) $(CCFLAGS) graph.cpp

graphIsomorph.o: graphIsomorph.hpp graphIsomorph.cpp graphHash.hpp graph.hpp
	$(CC) $(CCFLAGS) graphIsomorph.cpp

graphHashSpecialTester.o: graphHashSpecialTester.cpp
	$(CC) $(CCFLAGS) graphHashSpecialTester.cpp

graphHashBug1Tester.o: graphHashBug1Tester.cpp graphHash.hpp graph.hpp
	$(CC) $(CCFLAGS) graphHashBug1Tester.cpp

graphHashBug2Tester.o: graphHashBug2Tester.cpp graphHash.hpp graph.hpp
	$(CC) $(CCFLAGS) graphHashBug2Tester.cpp

graphHashRegularGraphTester.o: graphHashRegularGraphTester.cpp graphHash.hpp graph.hpp
	$(CC) $(CCFLAGS) graphHashRegularGraphTester.cpp

graphHashCaseTester.o: graphHashCaseTester.cpp graphHash.hpp graph.hpp
	$(CC) $(CCFLAGS) graphHashCaseTester.cpp

graph_ge.o: graph_ge.cpp graph_ge.h
	$(CC) $(CCFLAGS) graph_ge.cpp

md5.o: md5.h md5.cpp
	$(CC) $(CCFLAGS) md5.cpp

random.o: random.hpp random.cpp
	$(CC) $(CCFLAGS) random.cpp

fileio.o: fileio.h fileio.cpp
	$(CC) $(CCFLAGS) fileio.cpp

gettime.o: gettime.h gettime.cpp
	$(CC) $(CCFLAGS) gettime.cpp

clean:
	/bin/rm -f *.o

