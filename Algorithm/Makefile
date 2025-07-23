# Makefile for building both Algorithm plugins
# Produces:
#   sos/libAlgorithm_315634022.so
#   sos/libAlgorithmAlt_315634022.so

# Root of the Simulator workspace (for headers)
PROJECT_ROOT ?= ..

# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra -g \
              -I$(PROJECT_ROOT)/common \
              -I$(PROJECT_ROOT)/UserCommon \
              -Iinclude

# Platform detection for linker flags
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # macOS: Let the simulator resolve registrar symbols at runtime
    LDFLAGS := -shared -undefined dynamic_lookup
else
    # Linux: Standard shared library
    LDFLAGS := -shared -fPIC -lpthread
endif

# Build configuration
DEBUG ?= 1
ifeq ($(DEBUG),0)
    CXXFLAGS := $(subst -g,-O2 -DNDEBUG,$(CXXFLAGS))
endif

# Registration stub sources (common to both plugins)
REG_STUBS := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
             $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp

# Build and output directories
BUILD_DIR  := build
TARGET_DIR := sos

# Plugin 1 (primary algorithm)
PLUGIN1        := libAlgorithm_315634022.so
PLUGIN1_SRCS   := TankAlgorithm_315634022.cpp Player_315634022.cpp
PLUGIN1_OBJS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(PLUGIN1_SRCS))
PLUGIN1_TARGET := $(TARGET_DIR)/$(PLUGIN1)

# Plugin 2 (alternative algorithm)
PLUGIN2        := libAlgorithmAlt_315634022.so
PLUGIN2_SRCS   := TankAlgorithmAlt_315634022.cpp PlayerAlt_315634022.cpp
PLUGIN2_OBJS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(PLUGIN2_SRCS))
PLUGIN2_TARGET := $(TARGET_DIR)/$(PLUGIN2)

# All object files and dependencies
ALL_OBJS := $(PLUGIN1_OBJS) $(PLUGIN2_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

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
all: $(PLUGIN1_TARGET) $(PLUGIN2_TARGET)

# Build configuration targets
debug: DEBUG=1
debug: all

release: DEBUG=0
release: all

# Ensure build and output directories exist
$(BUILD_DIR) $(TARGET_DIR):
	$(Q)mkdir -p $@

# Compile .cpp to .o with dependency generation
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(ECHO) "  CXX     $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(ALL_DEPS)

# Build plugin 1
$(PLUGIN1_TARGET): $(PLUGIN1_OBJS) | $(TARGET_DIR)
	$(ECHO) "  LINK    $@"
	$(Q)$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@
	$(ECHO) "Primary algorithm plugin built: $@"

# Build plugin 2
$(PLUGIN2_TARGET): $(PLUGIN2_OBJS) | $(TARGET_DIR)
	$(ECHO) "  LINK    $@"
	$(Q)$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@
	$(ECHO) "Alternative algorithm plugin built: $@"

# Development utilities
check-vars:
	@echo "Build Configuration:"
	@echo "==================="
	@echo "PROJECT_ROOT:     $(PROJECT_ROOT)"
	@echo "CXX:              $(CXX)"
	@echo "CXXFLAGS:         $(CXXFLAGS)"
	@echo "LDFLAGS:          $(LDFLAGS)"
	@echo "DEBUG:            $(DEBUG)"
	@echo "Platform:         $(UNAME_S)"
	@echo ""
	@echo "Plugin 1 (Primary):"
	@echo "SOURCES:          $(PLUGIN1_SRCS)"
	@echo "OBJECTS:          $(PLUGIN1_OBJS)"
	@echo "TARGET:           $(PLUGIN1_TARGET)"
	@echo ""
	@echo "Plugin 2 (Alternative):"
	@echo "SOURCES:          $(PLUGIN2_SRCS)"
	@echo "OBJECTS:          $(PLUGIN2_OBJS)"
	@echo "TARGET:           $(PLUGIN2_TARGET)"
	@echo ""
	@echo "Registration stubs: $(REG_STUBS)"

# List source files and verify they exist
list-sources:
	@echo "Checking source files..."
	@echo "Plugin 1 sources:"
	@for src in $(PLUGIN1_SRCS); do \
		if [ -f "$$src" ]; then \
			echo "  ✓ $$src"; \
		else \
			echo "  ✗ $$src (missing)"; \
		fi \
	done
	@echo "Plugin 2 sources:"
	@for src in $(PLUGIN2_SRCS); do \
		if [ -f "$$src" ]; then \
			echo "  ✓ $$src"; \
		else \
			echo "  ✗ $$src (missing)"; \
		fi \
	done
	@echo "Registration stubs:"
	@for stub in $(REG_STUBS); do \
		if [ -f "$$stub" ]; then \
			echo "  ✓ $$stub"; \
		else \
			echo "  ✗ $$stub (missing)"; \
		fi \
	done

# Test plugin loading and dependencies
test-plugins: $(PLUGIN1_TARGET) $(PLUGIN2_TARGET)
	@echo "Testing plugin dependencies..."
	@echo ""
	@echo "=== Plugin 1: $(PLUGIN1) ==="
	@if command -v ldd >/dev/null 2>&1; then \
		echo "Dependencies:"; ldd $(PLUGIN1_TARGET) 2>/dev/null || echo "No dependencies or ldd unavailable"; \
	elif command -v otool >/dev/null 2>&1; then \
		echo "Dependencies:"; otool -L $(PLUGIN1_TARGET) 2>/dev/null || echo "No dependencies or otool unavailable"; \
	fi
	@echo "File info: $$(file $(PLUGIN1_TARGET))"
	@echo ""
	@echo "=== Plugin 2: $(PLUGIN2) ==="
	@if command -v ldd >/dev/null 2>&1; then \
		echo "Dependencies:"; ldd $(PLUGIN2_TARGET) 2>/dev/null || echo "No dependencies or ldd unavailable"; \
	elif command -v otool >/dev/null 2>&1; then \
		echo "Dependencies:"; otool -L $(PLUGIN2_TARGET) 2>/dev/null || echo "No dependencies or otool unavailable"; \
	fi
	@echo "File info: $$(file $(PLUGIN2_TARGET))"

# Build only plugin 1
plugin1: $(PLUGIN1_TARGET)

# Build only plugin 2
plugin2: $(PLUGIN2_TARGET)

# Install plugins to specified directory
install: $(PLUGIN1_TARGET) $(PLUGIN2_TARGET)
	@if [ -z "$(INSTALL_DIR)" ]; then \
		echo "Usage: make install INSTALL_DIR=/path/to/plugins"; \
		echo "Or set INSTALL_DIR environment variable"; \
		exit 1; \
	fi
	@mkdir -p $(INSTALL_DIR)
	@cp $(PLUGIN1_TARGET) $(PLUGIN2_TARGET) $(INSTALL_DIR)/
	@echo "Plugins installed to $(INSTALL_DIR)/"
	@echo "  - $(PLUGIN1)"
	@echo "  - $(PLUGIN2)"

# Help target
help:
	@echo "Algorithm Plugins Build System"
	@echo "=============================="
	@echo ""
	@echo "This Makefile builds two algorithm plugins:"
	@echo "  $(PLUGIN1)    - Primary algorithm"
	@echo "  $(PLUGIN2) - Alternative algorithm"
	@echo ""
	@echo "Targets:"
	@echo "  all           - Build both plugins (default)"
	@echo "  plugin1       - Build only primary plugin"
	@echo "  plugin2       - Build only alternative plugin"
	@echo "  debug         - Build with debug symbols (default)"
	@echo "  release       - Build optimized version"
	@echo "  clean         - Remove build artifacts"
	@echo "  rebuild       - Clean rebuild of both plugins"
	@echo "  test-plugins  - Test plugin dependencies and info"
	@echo "  check-vars    - Display build variables"
	@echo "  list-sources  - List and verify source files"
	@echo "  install       - Install plugins (requires INSTALL_DIR)"
	@echo "  help          - Show this help"
	@echo ""
	@echo "Variables:"
	@echo "  PROJECT_ROOT  - Path to simulator root (default: ..)"
	@echo "  DEBUG         - Enable debug build (0/1, default: 1)"
	@echo "  V             - Verbose output (0/1, default: 0)"
	@echo "  INSTALL_DIR   - Installation directory for install target"
	@echo ""
	@echo "Examples:"
	@echo "  make plugin1              # Build only primary plugin"
	@echo "  make release              # Build both plugins optimized"
	@echo "  make V=1                  # Build with verbose output"
	@echo "  make clean plugin2        # Clean build of alternative plugin"
	@echo "  make install INSTALL_DIR=../simulator/plugins"

# Rebuild everything
rebuild: clean all

# Clean target
clean:
	$(ECHO) "  CLEAN   $(BUILD_DIR) $(TARGET_DIR)"
	$(Q)rm -rf $(BUILD_DIR) $(TARGET_DIR)

# Phony targets
.PHONY: all debug release clean rebuild plugin1 plugin2 check-vars list-sources test-plugins install help

# # Makefile for building both Algorithm plugins
# # Produces:
# #   sos/libAlgorithm_315634022.so
# #   sos/libAlgorithmAlt_315634022.so

# # Root of the Simulator workspace (for headers)
# PROJECT_ROOT ?= ..

# # Compiler and flags
# CXX       := g++
# CXXFLAGS  := -std=c++17 -fPIC -Wall -Wextra \
#               -I$(PROJECT_ROOT)/common \
#               -I$(PROJECT_ROOT)/UserCommon \
#               -Iinclude
# # Let the simulator resolve registrar symbols at runtime
# LDFLAGS   := -shared -undefined dynamic_lookup

# # Registration stub sources (common to both plugins)
# REG_STUBS := $(PROJECT_ROOT)/Simulator/PlayerRegistration.cpp \
#              $(PROJECT_ROOT)/Simulator/TankAlgorithmRegistration.cpp

# # Output directory
# TARGET_DIR := sos

# # Plugin 1 (primary algorithm)
# PLUGIN1     := libAlgorithm_315634022.so
# PLUGIN1_SRCS:= TankAlgorithm_315634022.cpp Player_315634022.cpp
# PLUGIN1_OBJS:= $(PLUGIN1_SRCS:.cpp=.o)
# PLUGIN1_TARGET := $(TARGET_DIR)/$(PLUGIN1)

# # Plugin 2 (alternative algorithm)
# PLUGIN2     := libAlgorithmAlt_315634022.so
# PLUGIN2_SRCS:= TankAlgorithmAlt_315634022.cpp PlayerAlt_315634022.cpp
# PLUGIN2_OBJS:= $(PLUGIN2_SRCS:.cpp=.o)
# PLUGIN2_TARGET := $(TARGET_DIR)/$(PLUGIN2)

# .PHONY: all clean
# all: $(PLUGIN1_TARGET) $(PLUGIN2_TARGET)

# # Ensure output directory exists
# $(TARGET_DIR):
# 	mkdir -p $(TARGET_DIR)

# # Build plugin 1
# $(PLUGIN1_TARGET): $(PLUGIN1_OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@

# # Build plugin 2
# $(PLUGIN2_TARGET): $(PLUGIN2_OBJS) | $(TARGET_DIR)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(REG_STUBS) -o $@

# # Compile any .cpp to .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(PLUGIN1_OBJS) $(PLUGIN2_OBJS)
# 	rm -rf $(TARGET_DIR)



















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
