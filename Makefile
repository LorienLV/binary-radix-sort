CXX:=g++

CXXFLAGS:=-std=c++20 -O3

.DEFAULT_GOAL:=
.PHONY: all
all:
	$(CXX) $(CXXFLAGS) bin_radix_sort.cpp -o bin_radix_sort