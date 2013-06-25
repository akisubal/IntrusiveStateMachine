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


build_test:	test.cpp test_mathematical_set.hpp
	$(CXX) -o $@ -I$(GTEST_DIR)/include $(GTEST_DIR)/libgtest.a $(GTEST_DIR)/libgtest_main.a test.cpp



clean:
	rm -f *.o
