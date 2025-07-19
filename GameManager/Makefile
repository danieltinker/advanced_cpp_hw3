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

# GameManager/Makefile

# # The simulator’s common/ folder is one level up
# PROJECT_ROOT ?= ..

# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -Wall -Wextra \
#             -I$(PROJECT_ROOT) \
#             -Iinclude/GameManager_315634022
# LDFLAGS  := -shared

# SRCDIR   := src
# SRCS     := $(wildcard $(SRCDIR)/*.cpp)
# OBJS     := $(patsubst $(SRCDIR)/%.cpp,$(SRCDIR)/%.o,$(SRCS))

# TARGET   := GameManager_315634022.so

# all: $(TARGET)

# $(SRCDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(TARGET): $(OBJS)
# 	$(CXX) $(LDFLAGS) $^ -o $@

# clean:
# 	rm -f $(SRCDIR)/*.o $(TARGET)

# GameManager/Makefile
# GameManager/Makefile

# “common/” lives one level up from GameManager/
PROJECT_ROOT ?= ..

CXX      := g++
CXXFLAGS := -std=c++17 -g -O0 -fPIC -Wall -Wextra \
            -I$(PROJECT_ROOT)/common \
            -Iinclude/GameManager_315634022
LDFLAGS  := -shared

SRCDIR   := src
SRCS     := $(wildcard src/*.cpp) \
           $(PROJECT_ROOT)/Simulator/GameManagerRegistration.cpp \
           $(PROJECT_ROOT)/Simulator/GameManagerRegistrar.cpp
OBJS     := $(patsubst $(SRCDIR)/%.cpp,$(SRCDIR)/%.o,$(SRCS))

TARGET   := GameManager_315634022.so


all: $(TARGET)

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
