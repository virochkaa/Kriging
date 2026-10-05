CXX = clang++
CXXFLAGS = -std=c++17 -Iinclude

all: surrogates

surrogates: src/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f surrogates