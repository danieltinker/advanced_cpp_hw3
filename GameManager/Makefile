# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -g -fPIC -I../common -I../UserCommon -I./include
#hagai flags (32299...):
# CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -g -fPIC

LDFLAGS = -shared 
#-undefined dynamic_lookup 

# === Build Folder and Target Executable ===
BUILD_DIR = build
TARGET = GameManager_315634022.so

# === List of Source Files ===
SRCS = \
	src/Tank.cpp \
	src/Board.cpp \
	src/GameState.cpp \
	src/MySatelliteView.cpp \
	src/FinalBoardView.cpp \
	src/GameManager_315634022.cpp

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