CXX=g++
TARGET=ism
CXXFLAGS=-Wall -O2
LD_LIBRARIES=-lpthread 

TEST_FILES=test_mathematical_set.hpp test_intrusive_state_machine.hpp

.PHONY:	clean run_test

ism:
	$(CXX) -o $@ main.o

test: build_test
	./$<


main.o:	main.cpp intrusive_state_machine.hpp mathematical_set.hpp
	$(CXX) -c main.cpp


build_test:	test.cpp mathematical_set.hpp $(TEST_FILES)
	$(CXX) -o $@ test.cpp -I$(GTEST_DIR)/include $(GTEST_DIR)/libgtest.a $(GTEST_DIR)/libgtest_main.a $(LD_LIBRARIES) 




clean:
	rm -f *.o
