# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -I../common -I../UserCommon
LDFLAGS = -rdynamic

# === Build Folder and Target Executable ===
BUILD_DIR = build
TARGET = simulator_315634022

# === List of Source Files ===
SRCS = \
    main.cpp \
	ArgParser.cpp \
	AlgorithmRegistrar.cpp \
	GameManagerRegistrar.cpp \
	ThreadPool.cpp \
	GameManagerRegistration.cpp \
	TankAlgorithmRegistration.cpp \
	PlayerRegistration.cpp \


	
# === Convert SRCS paths to OBJS paths in build ===
OBJS := $(SRCS)
OBJS := $(patsubst ../%, %, $(OBJS)) # remove ../ before creating build paths
OBJS := $(patsubst %, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))

# === Rule to Create the Executable ===
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

# === Compilation Rules ===
$(BUILD_DIR)/%.o: ../%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Clean Rules ===
clean:
	rm -r -f $(BUILD_DIR)
	rm -f $(TARGET)

# Optional: automatic clean on make
.PHONY: clean