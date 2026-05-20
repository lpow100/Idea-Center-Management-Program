CXX = g++
SRC := $(wildcard src/*.cpp)
OBJS := $(SRC:.cpp=.o)

CXXFLAGS = -std=c++17 -Wall -Iphc-winner-argon2-20190702/include -Iinclude

LDLIBS = -lpqxx -lpq -lfmt

LDFLAGS = 

ifeq ($(OS), Windows_NT)
	VCPKG_ROOT := C:/Users/mrpee/Documents/Programming/vcpkg
	CXXFLAGS = -std=c++17 -Wall \
            -Iphc-winner-argon2-20190702/include \
            -Iinclude \
            -I$(VCPKG_ROOT)/installed/x64-mingw-dynamic/include
	LDFLAGS = -L$(VCPKG_ROOT)/installed/x64-mingw-dynamic/lib
endif

all: main

main: $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS) $(LDFLAGS) phc-winner-argon2-20190702/libargon2.a

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f *.o

