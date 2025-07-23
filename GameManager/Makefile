# Makefile for building the GameManager plugin (GameManager_315634022.so)

# Allow overriding the simulator workspace root
PROJECT_ROOT ?= ..

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -fPIC -Wall -Wextra -g \
            -I$(PROJECT_ROOT)/common \
            -Iinclude/GameManager_315634022

# Platform detection for linker flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS: defer undefined symbols to runtime
    LDFLAGS := -shared -undefined dynamic_lookup
else
    # Linux: standard shared library
    LDFLAGS := -shared -fPIC -lpthread
endif

# Build configuration
DEBUG ?= 1
ifeq ($(DEBUG),0)
    CXXFLAGS := $(subst -g,-O2 -DNDEBUG,$(CXXFLAGS))
endif

# Directories
SRC_DIR     := src
BUILD_DIR   := build
TARGET_DIR  := sos
INCLUDE_DIR := include/GameManager_315634022

# Plugin name and target
PLUGIN_NAME := GameManager_315634022.so
TARGET      := $(TARGET_DIR)/$(PLUGIN_NAME)

# Source and object files with dependency tracking
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Header dependencies
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(INCLUDE_DIR)/*.hpp)
COMMON_HEADERS := $(wildcard $(PROJECT_ROOT)/common/*.h) $(wildcard $(PROJECT_ROOT)/common/*.hpp)

# Verbose mode
V ?= 0
ifeq ($(V),0)
    Q := @
    ECHO := @echo
else
    Q :=
    ECHO := @true
endif

# Default target
all: $(TARGET)

# Build configuration targets
debug: DEBUG=1
debug: $(TARGET)

release: DEBUG=0
release: $(TARGET)

# Ensure build and target dirs exist
$(BUILD_DIR) $(TARGET_DIR):
	$(Q)mkdir -p $@

# Compile .cpp -> .o in build dir with dependency generation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(ECHO) "  CXX     $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Link plugin
$(TARGET): $(OBJS) | $(TARGET_DIR)
	$(ECHO) "  LINK    $@"
	$(Q)$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@
	$(ECHO) "Plugin built successfully: $@"

# Development utilities
check-vars:
	@echo "PROJECT_ROOT: $(PROJECT_ROOT)"
	@echo "CXX:          $(CXX)"
	@echo "CXXFLAGS:     $(CXXFLAGS)"
	@echo "LDFLAGS:      $(LDFLAGS)"
	@echo "SRCS:         $(SRCS)"
	@echo "OBJS:         $(OBJS)"
	@echo "TARGET:       $(TARGET)"
	@echo "Platform:     $(UNAME_S)"

# List source files
list-sources:
	@echo "Source files found:"
	@for src in $(SRCS); do echo "  $$src"; done
	@echo "Header files found:"
	@for hdr in $(HEADERS); do echo "  $$hdr"; done

# Check if plugin loads correctly (requires simulator)
test-load: $(TARGET)
	@echo "Testing plugin load (requires simulator in PATH)..."
	@if command -v ldd >/dev/null 2>&1; then \
		echo "Library dependencies:"; \
		ldd $(TARGET) 2>/dev/null || echo "  (No dependencies or ldd unavailable)"; \
	elif command -v otool >/dev/null 2>&1; then \
		echo "Library dependencies:"; \
		otool -L $(TARGET) 2>/dev/null || echo "  (No dependencies or otool unavailable)"; \
	fi
	@echo "Plugin file info:"
	@file $(TARGET)

# Install plugin to a specific directory
install: $(TARGET)
	@if [ -z "$(INSTALL_DIR)" ]; then \
		echo "Usage: make install INSTALL_DIR=/path/to/plugins"; \
		echo "Or set INSTALL_DIR environment variable"; \
		exit 1; \
	fi
	@mkdir -p $(INSTALL_DIR)
	@cp $(TARGET) $(INSTALL_DIR)/
	@echo "Plugin installed to $(INSTALL_DIR)/$(PLUGIN_NAME)"

# Help target
help:
	@echo "GameManager Plugin Build System"
	@echo "==============================="
	@echo ""
	@echo "Targets:"
	@echo "  all           - Build plugin (default)"
	@echo "  debug         - Build with debug symbols (default)"
	@echo "  release       - Build optimized version"
	@echo "  clean         - Remove build artifacts"
	@echo "  test-load     - Test plugin dependencies and info"
	@echo "  check-vars    - Display build variables"
	@echo "  list-sources  - List discovered source/header files"
	@echo "  install       - Install plugin (requires INSTALL_DIR)"
	@echo "  help          - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  PROJECT_ROOT  - Path to simulator root (default: ..)"
	@echo "  DEBUG         - Enable debug build (0/1, default: 1)"
	@echo "  V             - Verbose output (0/1, default: 0)"
	@echo "  INSTALL_DIR   - Installation directory for install target"
	@echo ""
	@echo "Examples:"
	@echo "  make release              # Build optimized version"
	@echo "  make V=1                  # Build with verbose output"
	@echo "  make clean all            # Clean rebuild"
	@echo "  make install INSTALL_DIR=../plugins"

# Rebuild everything
rebuild: clean all

# Clean target
clean:
	$(ECHO) "  CLEAN   $(BUILD_DIR) $(TARGET_DIR)"
	$(Q)rm -rf $(BUILD_DIR) $(TARGET_DIR)

# Phony targets
.PHONY: all debug release clean rebuild check-vars list-sources test-load install help
# # Makefile for building the GameManager plugin (GameManager_315634022.so)

# # Allow overriding the simulator workspace root
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -Wall -Wextra \
#             -I$(PROJECT_ROOT)/common \
#             -Iinclude/GameManager_315634022

# # Linker flags: shared lib, defer undefined symbols to simulator
# LDFLAGS  := -shared -undefined dynamic_lookup

# # Directories
# SRC_DIR     := src
# BUILD_DIR   := build
# TARGET_DIR  := sos

# # Plugin name
# PLUGIN_NAME := GameManager_315634022.so
# TARGET      := $(TARGET_DIR)/$(PLUGIN_NAME)

# # Source and object files
# SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# .PHONY: all clean

# all: $(TARGET)

# # Ensure build and target dirs exist
# $(BUILD_DIR) $(TARGET_DIR):
# 	@mkdir -p $@

# # Compile .cpp -> .o in build dir
# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Link plugin
# $(TARGET): $(OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

# clean:
# 	rm -rf $(BUILD_DIR) $(TARGET_DIR)








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




