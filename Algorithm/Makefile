CXX       := g++
CXXFLAGS  := -std=c++17 -fPIC -g -O0 -I../common -I../UserCommon

LDFLAGS   := -bundle -undefined dynamic_lookup

ID        := 315634022
LIB       := libAlgorithm_$(ID).so
OBJ       := TankAlgorithm_$(ID).o Player_$(ID).o

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
