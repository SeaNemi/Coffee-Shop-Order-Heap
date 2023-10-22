CXX = g++
CXXFLAGS = -Wall -g
IODIR =../../proj0_IO/

proj0: cqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) cqueue.o mytest.cpp -o proj3

cqueue.o: cqueue.h cqueue.cpp
	$(CXX) $(CXXFLAGS) -c cqueue.cpp

clean:
	rm *.o*
	rm *~

run:
	./proj3

val:
	valgrind ./proj3