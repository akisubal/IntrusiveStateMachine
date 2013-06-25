CXX=g++
TARGET=ism
CXXFLAGS=-Wall -O2

.PHONY:	clean run_test

ism:
	$(CXX) -o $@ main.o

test: build_test
	./$<


main.o:	main.cpp intrusive_state_machine.hpp mathematical_set.hpp
	$(CXX) -c main.cpp


build_test:	test.cpp
	$(CXX) -o $@ -I$(GTEST_DIR)/include $(GTEST_DIR)/libgtest.a test.cpp



clean:
	rm -f *.o
