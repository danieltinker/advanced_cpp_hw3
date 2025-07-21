# Compiler and basic flags
CXX             := g++
CXXFLAGS        := -std=c++17 -fPIC -g -O0 -Wall -Wextra -I. -I../common -I../UserCommon

# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS specific flags
    LDFLAGS_SO      := -dynamiclib -install_name @rpath/libsimreg.so
    EXPORT_SYMS     := -Wl,-export_dynamic
    RPATH           := -Wl,-rpath,@loader_path
else
    # Linux specific flags
    LDFLAGS_SO      := -shared -fPIC
    EXPORT_SYMS     := -Wl,--export-dynamic
    RPATH           := -Wl,-rpath,'$$ORIGIN'
endif

# Build configuration
BUILD_DIR       := build
OUTPUT_DIR      := .
DEBUG           ?= 1
ifeq ($(DEBUG),0)
    CXXFLAGS    := $(subst -g -O0,-O2 -DNDEBUG,$(CXXFLAGS))
endif

# Linking flags
LDLIBS_TEST     := -L$(OUTPUT_DIR) -lsimreg -ldl

# Source files organized by component
REG_SRCS        := AlgorithmRegistrar.cpp \
                   PlayerRegistration.cpp \
                   TankAlgorithmRegistration.cpp \
                   GameManagerRegistration.cpp \
                   GameManagerRegistrar.cpp

TP_SRCS         := ThreadPool.cpp
AP_SRCS         := ArgParser.cpp
TEST_SRCS       := test_dynamic_load.cpp
MAIN_SRCS       := main.cpp

# Object files with build directory
REG_OBJS        := $(REG_SRCS:%.cpp=$(BUILD_DIR)/%.o)
TP_OBJS         := $(TP_SRCS:%.cpp=$(BUILD_DIR)/%.o)
AP_OBJS         := $(AP_SRCS:%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJS       := $(TEST_SRCS:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJS       := $(MAIN_SRCS:%.cpp=$(BUILD_DIR)/%.o)

ALL_OBJS        := $(REG_OBJS) $(TP_OBJS) $(AP_OBJS) $(TEST_OBJS) $(MAIN_OBJS)

# Target names - executables go to current directory
LIB             := $(OUTPUT_DIR)/libsimreg.so
TEST_TARGET     := $(OUTPUT_DIR)/test_dynamic_load
SIM_TARGET      := $(OUTPUT_DIR)/simulator_315634022

# Default target
all: $(LIB) $(TEST_TARGET) $(SIM_TARGET)

# Create directories
$(BUILD_DIR) $(OUTPUT_DIR):
	@mkdir -p $@

# Generic rule for .cpp → .o with dependency generation
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(ALL_OBJS:.o=.d)

# Build the registrar library
$(LIB): $(REG_OBJS) | $(OUTPUT_DIR)
	$(CXX) $(LDFLAGS_SO) -o $@ $^

# Build test driver
$(TEST_TARGET): $(TEST_OBJS) $(LIB) | $(OUTPUT_DIR)
	$(CXX) -o $@ $(TEST_OBJS) $(LDLIBS_TEST) $(RPATH)

# Build simulator
$(SIM_TARGET): $(MAIN_OBJS) $(AP_OBJS) $(TP_OBJS) $(LIB) | $(OUTPUT_DIR)
	$(CXX) $(EXPORT_SYMS) -o $@ $(MAIN_OBJS) $(AP_OBJS) $(TP_OBJS) $(LDLIBS_TEST) $(RPATH)

# Development targets
debug: DEBUG=1
debug: all

release: DEBUG=0
release: all

# Install target (optional)
install: all
	@echo "Install target not implemented - specify installation directory"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build all targets (default)"
	@echo "  debug     - Build with debug flags (default)"
	@echo "  release   - Build with optimizations"
	@echo "  clean     - Remove all build artifacts"
	@echo "  help      - Show this help message"

# Clean target
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LIB) $(TEST_TARGET) $(SIM_TARGET)

# Verbose mode
V ?= 0
ifeq ($(V),0)
    Q := @
else
    Q :=
endif

# Override compilation rule for verbose output
ifeq ($(V),1)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
else
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo "  CXX     $<"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
endif

# Phony targets
.PHONY: all debug release clean help install


# CXX             := g++
# CXXFLAGS        := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon

# # build libsimreg.so as a macOS dynamic library
# LDFLAGS_SO      := -dynamiclib -install_name @rpath/libsimreg.so

# # for linking the test driver and simulator executable
# LDLIBS_TEST     := -L. -lsimreg -ldl
# # on macOS clang/ld, use -export_dynamic to expose all symbols
# EXPORT_SYMS     := -Wl,-export_dynamic
# # rpath so the loader always looks next to the exe for libsimreg.so
# RPATH           := -Wl,-rpath,@loader_path

# # registration glue
# SRC             := AlgorithmRegistrar.cpp   \
#                    PlayerRegistration.cpp   \
#                    TankAlgorithmRegistration.cpp \
#                    GameManagerRegistration.cpp \
#                    GameManagerRegistrar.cpp
# OBJ             := $(SRC:.cpp=.o)

# LIB             := libsimreg.so

# # thread pool
# TP_SRCS         := ThreadPool.cpp
# TP_OBJS         := ThreadPool.o

# # argument parser
# AP_SRCS         := ArgParser.cpp
# AP_OBJS         := ArgParser.o

# all: $(LIB) test_dynamic_load simulator_315634022

# # generic rule for .cpp → .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # build the registrar as a macOS dylib
# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS_SO) -o $@ $^

# # build the thread‐pool object
# ThreadPool.o: ThreadPool.cpp ThreadPool.hpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # build the argument‐parser object
# ArgParser.o: ArgParser.cpp ArgParser.hpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # compile the test driver
# test_dynamic_load.o: test_dynamic_load.cpp AlgorithmRegistrar.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # link test driver with libsimreg and embed rpath
# test_dynamic_load: test_dynamic_load.o $(LIB)
# 	$(CXX) -o $@ test_dynamic_load.o $(LDLIBS_TEST) $(RPATH)

# # compile the simulator driver
# main.o: main.cpp ArgParser.hpp AlgorithmRegistrar.h GameManagerRegistrar.h ThreadPool.hpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # link simulator: include parser, threadpool, and registrar lib
# simulator_315634022: main.o ArgParser.o ThreadPool.o $(LIB)
# 	$(CXX) $(EXPORT_SYMS) -o $@ main.o ArgParser.o ThreadPool.o $(LDLIBS_TEST) $(RPATH)

# clean:
# 	rm -f $(OBJ) $(LIB) test_dynamic_load main.o ArgParser.o ThreadPool.o simulator_315634022

# .PHONY: all clean
