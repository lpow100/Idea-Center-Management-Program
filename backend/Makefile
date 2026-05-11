CXX = g++
SRC := $(wildcard *.cpp)
OBJS := $(SRC:.cpp=.o)

CXXFLAGS = -std=c++17 -Wall -Iphc-winner-argon2-20190702/include

LDLIBS = -lpqxx -lpq

all: main clean

main: $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS) phc-winner-argon2-20190702/libargon2.a

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.o

