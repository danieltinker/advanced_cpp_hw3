# Makefile for building the GameManager plugin (GameManager_315634022.so)

# Allow overriding the simulator workspace root
PROJECT_ROOT ?= ..

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -fPIC -Wall -Wextra \
            -I$(PROJECT_ROOT)/common \
            -Iinclude/GameManager_315634022

# Linker flags: shared lib, defer undefined symbols to simulator
LDFLAGS  := -shared -undefined dynamic_lookup

# Directories
SRC_DIR     := src
BUILD_DIR   := build
TARGET_DIR  := sos

# Plugin name
PLUGIN_NAME := GameManager_315634022.so
TARGET      := $(TARGET_DIR)/$(PLUGIN_NAME)

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

# Ensure build and target dirs exist
$(BUILD_DIR) $(TARGET_DIR):
	@mkdir -p $@

# Compile .cpp -> .o in build dir
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link plugin
$(TARGET): $(OBJS) | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_DIR)


# # Makefile for building the GameManager plugin (GameManager_315634022.so)

# # Root of the Simulator workspace (for headers)
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -Wall -Wextra \
#             -I$(PROJECT_ROOT)/common \
#             -Iinclude/GameManager_315634022
# # Allow undefined refs to be resolved by the simulator executable
# LDFLAGS  := -shared -undefined dynamic_lookup

# # Output
# TARGET_DIR := sos
# TARGET     := $(TARGET_DIR)/GameManager_315634022.so

# # Source files
# SRCS := \
#     src/Board.cpp \
#     src/GameManager.cpp \
#     src/GameState.cpp \
#     src/MyBattleInfo.cpp \
#     src/MySatelliteView.cpp \
#     src/Tank.cpp

# OBJS := $(SRCS:.cpp=.o)

# .PHONY: all clean
# all: $(TARGET)

# # Ensure output directory exists
# $(TARGET_DIR):
# 	mkdir -p $(TARGET_DIR)

# # Link the plugin
# $(TARGET): $(OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

# # Compile each .cpp
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	rm -rf $(TARGET_DIR)


















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

# # “common/” lives one level up from GameManager/
# PROJECT_ROOT ?= ..

# CXX      := g++
# CXXFLAGS := -std=c++17 -g -O0 -fPIC -Wall -Wextra \
#             -I$(PROJECT_ROOT)/common \
#             -Iinclude/GameManager_315634022
# LDFLAGS  := -shared

# SRCDIR   := src
# SRCS     := $(wildcard src/*.cpp) \
#            $(PROJECT_ROOT)/Simulator/GameManagerRegistration.cpp \
#            $(PROJECT_ROOT)/Simulator/GameManagerRegistrar.cpp
# OBJS     := $(patsubst $(SRCDIR)/%.cpp,$(SRCDIR)/%.o,$(SRCS))

# TARGET   := GameManager_315634022.so


# all: $(TARGET)

# $(SRCDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(TARGET): $(OBJS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# clean:
# 	rm -f $(SRCDIR)/*.o $(TARGET)




