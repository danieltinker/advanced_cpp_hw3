# # GameManager/Makefile

# # CXX       := /usr/local/bin/g++-15
# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -g -O0 -I../common -I../UserCommon

# # also a bundle
# LDFLAGS   := -bundle -undefined dynamic_lookup

# ID        := 315634022
# LIB       := libGameManager_$(ID).so
# OBJ       := GameManager_$(ID).o

# all: $(LIB)

# # $(LIB): $(OBJ)
# # 	$(CXX) $(LDFLAGS) -o $@ $^ -L../Simulator -lsimreg

# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS) -o $@ $^

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o *.so

# .PHONY: all clean



# CXX       := g++
# CXXFLAGS  := -fPIC -std=c++17 -I../common -I../UserCommon
# LDFLAGS   := -shared
# LIBS      := -L../Simulator -lsimreg

# # output directory for .so files
# LIBDIR    := sos

# all: $(LIBDIR) \
#      $(LIBDIR)/libGameManager_315634022.so

# # ensure the output directory exists
# $(LIBDIR):
# 	mkdir -p $@

# $(LIBDIR)/libGameManager_315634022.so: GameManager_315634022.o
# 	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# # compile the single GameManager .cpp
# GameManager_315634022.o: GameManager_315634022.cpp GameManager_315634022.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -rf *.o *.so $(LIBDIR)

# .PHONY: all clean


# GameManager/Makefile

CXX        := g++
CXXFLAGS   := -std=c++17 -fPIC -Wall -Wextra \
               -I$(PROJECT_ROOT)/common \
               -Iinclude
LDFLAGS    := -shared

SRCDIR     := src
INCDIR     := include

SRCS       := $(wildcard $(SRCDIR)/*.cpp)
OBJS       := $(SRCS:$(SRCDIR)/%.cpp=$(SRCDIR)/%.o)

TARGET     := GameManager_315634022.so

all: $(TARGET)

# Compile each .cpp into .o
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link to shared object
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
