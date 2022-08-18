CXX = g++
CXXFLAGS = -Wall -std=c++20

test: prime tuple_traits
	./prime
	./tuple_traits

prime: prime.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

tuple_traits: tuple_traits.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	del /f *.exe
