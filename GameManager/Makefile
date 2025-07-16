# GameManager/Makefile

# CXX       := /usr/local/bin/g++-15
CXX       := g++
CXXFLAGS  := -std=c++17 -fPIC -g -O0 -I../common -I../UserCommon

# also a bundle
LDFLAGS   := -bundle -undefined dynamic_lookup

ID        := 315634022
LIB       := libGameManager_$(ID).so
OBJ       := GameManager_$(ID).o

all: $(LIB)

# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS) -o $@ $^ -L../Simulator -lsimreg

$(LIB): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o *.so

.PHONY: all clean
