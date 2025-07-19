# Makefile for building both Algorithm plugins
# Produces:
#   sos/libAlgorithm_315634022.so
#   sos/libAlgorithmAlt_315634022.so

# Root of the Simulator workspace (for headers)
PROJECT_ROOT ?= ..

# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra \
              -I$(PROJECT_ROOT)/common \
              -I$(PROJECT_ROOT)/UserCommon \
              -Iinclude
# Let the simulator resolve registrar symbols at runtime
LDFLAGS   := -shared -undefined dynamic_lookup

# Registration stub sources (common to both plugins)
REG_STUBS := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
             $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp

# Output directory
TARGET_DIR := sos

# Plugin 1 (primary algorithm)
PLUGIN1     := libAlgorithm_315634022.so
PLUGIN1_SRCS:= TankAlgorithm_315634022.cpp Player_315634022.cpp
PLUGIN1_OBJS:= $(PLUGIN1_SRCS:.cpp=.o)
PLUGIN1_TARGET := $(TARGET_DIR)/$(PLUGIN1)

# Plugin 2 (alternative algorithm)
PLUGIN2     := libAlgorithmAlt_315634022.so
PLUGIN2_SRCS:= TankAlgorithmAlt_315634022.cpp PlayerAlt_315634022.cpp
PLUGIN2_OBJS:= $(PLUGIN2_SRCS:.cpp=.o)
PLUGIN2_TARGET := $(TARGET_DIR)/$(PLUGIN2)

.PHONY: all clean
all: $(PLUGIN1_TARGET) $(PLUGIN2_TARGET)

# Ensure output directory exists
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

# Build plugin 1
$(PLUGIN1_TARGET): $(PLUGIN1_OBJS) | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@

# Build plugin 2
$(PLUGIN2_TARGET): $(PLUGIN2_OBJS) | $(TARGET_DIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@

# Compile any .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(PLUGIN1_OBJS) $(PLUGIN2_OBJS)
	rm -rf $(TARGET_DIR)







# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -g -O0 -I../common -I../UserCommon

# LDFLAGS   := -bundle -undefined dynamic_lookup

# ID        := 315634022
# LIB       := libAlgorithm_$(ID).so
# OBJ       := TankAlgorithm_$(ID).o Player_$(ID).o

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

#################################################

# CXX       = g++
# CXXFLAGS  = -fPIC -std=c++17 -I../common -I../UserCommon
# LDFLAGS   = -shared
# LIBS      = -L../Simulator -lsimreg

# # your original stubs:
# ALGO1_OBJS = TankAlgorithm_315634022.o Player_315634022.o
# # the new stubs:
# ALGO2_OBJS = TankAlgorithmAlt_315634022.o PlayerAlt_315634022.o

# all: libAlgorithm_315634022.so libAlgorithmAlt_315634022.so

# libAlgorithm_315634022.so: $(ALGO1_OBJS)
# 	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# libAlgorithmAlt_315634022.so: $(ALGO2_OBJS)
# 	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# # compile rules for all .cpp → .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f *.o *.so

# .PHONY: all clean


####################

# CXX       := g++
# CXXFLAGS  := -fPIC -std=c++17 -I../common -I../UserCommon
# LDFLAGS   := -shared
# LIBS      := -L../Simulator -lsimreg

# # output directory for .so files
# LIBDIR    := sos

# # object sets for each plugin
# ALGO1_OBJS := TankAlgorithm_315634022.o Player_315634022.o
# ALGO2_OBJS := TankAlgorithmAlt_315634022.o PlayerAlt_315634022.o

# all: $(LIBDIR) \
#      $(LIBDIR)/libAlgorithm_315634022.so \
#      $(LIBDIR)/libAlgorithmAlt_315634022.so

# # ensure the output directory exists
# $(LIBDIR):
# 	mkdir -p $@

# # first algorithm .so
# $(LIBDIR)/libAlgorithm_315634022.so: $(ALGO1_OBJS)
# 	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# # second algorithm .so
# $(LIBDIR)/libAlgorithmAlt_315634022.so: $(ALGO2_OBJS)
# 	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# # generic rule for all .cpp -> .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -rf *.o *.so $(LIBDIR)

# .PHONY: all clean


# Makefile for building the Algorithm plugin (libAlgorithm_315634022.so)

# # Root of the Simulator workspace (where common/ and Simulator/ live)
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra \
#               -I$(PROJECT_ROOT)/common \
#               -I$(PROJECT_ROOT)/UserCommon \
#               -Iinclude
# LDFLAGS   := -shared

# # Your algorithm sources
# SOURCES   := TankAlgorithm_315634022.cpp \
#              Player_315634022.cpp
# OBJS      := $(SOURCES:.cpp=.o)

# # Simulator registration + registrar sources (compile-in to avoid -lsimreg)
# SIM_SRCS  := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
#              $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp \
#              $(PROJECT_ROOT)/Simulator/AlgorithmRegistrar.cpp

# # Output directory and target
# TARGET_DIR := sos
# TARGET      := $(TARGET_DIR)/libAlgorithm_315634022.so

# .PHONY: all clean

# all: $(TARGET)

# # Ensure output directory exists
# $(TARGET_DIR):
# 	mkdir -p $(TARGET_DIR)

# # Link everything into the shared object
# $(TARGET): $(OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(SIM_SRCS) -o $@

# # Compile each .cpp into .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	rm -rf $(TARGET_DIR)


# # ===== Option 1: Rely on simulator executable to provide registrar symbols =====

# # Root of the Simulator workspace (where common/ and Simulator/ live)
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra \
#               -I$(PROJECT_ROOT)/common \
#               -I$(PROJECT_ROOT)/UserCommon \
#               -Iinclude
# # Allow undefined symbols to be resolved at runtime by host
# LDFLAGS   := -shared -undefined dynamic_lookup

# # Your algorithm sources
# SOURCES   := TankAlgorithm_315634022.cpp \
#              Player_315634022.cpp
# OBJS      := $(SOURCES:.cpp=.o)

# # Plugin registration stub sources only
# SIM_SRCS  := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
#              $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp

# # Output directory and target
# TARGET_DIR := sos
# TARGET      := $(TARGET_DIR)/libAlgorithm_315634022.so

# .PHONY: all clean
# all: $(TARGET)

# $(TARGET_DIR):
# 	mkdir -p $(TARGET_DIR)

# $(TARGET): $(OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(SIM_SRCS) -o $@

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	rm -rf $(TARGET_DIR)





# # ===== Option 2: Link against libsimreg shared library =====

# # If you prefer explicit linking, first build libsimreg in the Simulator folder:
# # In Simulator/Makefile:
# #
# # LIBSIMREG_TARGET := libsimreg.dylib
# # LIBSIMREG_SRCS   := GameManagerRegistrar.cpp AlgorithmRegistrar.cpp
# #
# # $(LIBSIMREG_TARGET): $(LIBSIMREG_SRCS)
# #   $(CXX) $(CXXFLAGS) -shared $^ -o $@
# #
# # Then in this plugin Makefile:

# # Root of the Simulator workspace
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra \
#               -I$(PROJECT_ROOT)/common \
#               -I$(PROJECT_ROOT)/UserCommon \
#               -Iinclude
# LDFLAGS   := -shared

# # Your algorithm sources
# SOURCES   := TankAlgorithm_315634022.cpp \
#              Player_315634022.cpp
# OBJS      := $(SOURCES:.cpp=.o)

# # Registration stubs only
# SIM_SRCS  := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
#              $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp

# # Link against libsimreg.dylib
# SIM_LIB   := -L$(PROJECT_ROOT)/Simulator -lsimreg

# # Output directory and target
# TARGET_DIR := sos
# TARGET      := $(TARGET_DIR)/libAlgorithm_315634022.so

# .PHONY: all clean
# all: $(TARGET)

# $(TARGET_DIR):
# 	mkdir -p $(TARGET_DIR)

# $(TARGET): $(OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(SIM_SRCS) $(SIM_LIB) -o $@

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS)
# 	rm -rf $(TARGET_DIR)
