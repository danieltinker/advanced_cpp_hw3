# # Simulator/Makefile

# CXX         := g++
# CXXFLAGS    := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon
# LDFLAGS_SO  := -shared
# LDLIBS      := -ldl

# # The registration library that plugins will link against
# LIB          := libsimreg.so
# OBJS         := AlgorithmRegistrar.o \
#                 PlayerRegistration.o \
#                 TankAlgorithmRegistration.o \
#                 GameManagerRegistration.o \
# 				GameManagerRegistrar.o

# # Test executable
# TEST_EXE     := test_dynamic_load
# TEST_OBJ     := test_dynamic_load.o

# .PHONY: all clean

# all: $(LIB) $(TEST_EXE)

# # Build the shared registration lib
# $(LIB): $(OBJS)
# 	$(CXX) $(LDFLAGS_SO) -o $@ $^

# # Compile the simulator’s own .cpp files
# %.o: %.cpp %.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Build the test binary, linking against libsimreg and libAlgorithm/.so’s
# $(TEST_EXE): $(TEST_OBJ) $(LIB)
# 	$(CXX) -rdynamic -o $@ $< -L. -lsimreg $(LDLIBS)

# $(TEST_OBJ): test_dynamic_load.cpp AlgorithmRegistrar.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS) $(LIB) $(TEST_OBJ) $(TEST_EXE)

###############################################################################################################
# # Simulator/Makefile

# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon
# # LDFLAGS  := -shared
# LDFLAGS  := -dynamiclib -install_name @rpath/libsimreg.so
# LDLIBS   := -ldl
# RPATH    := -Wl,-rpath,@loader_path

# # sources & targets
# SRC := AlgorithmRegistrar.cpp \
#        PlayerRegistration.cpp \
#        TankAlgorithmRegistration.cpp \
#        GameManagerRegistration.cpp \
#        GameManagerRegistrar.cpp
# OBJ := $(SRC:.cpp=.o)
# LIB := libsimreg.so

# all: $(LIB) test_dynamic_load

# # compile each .cpp → .o
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # build shared registration library
# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS) -o $@ $^

# # compile the test driver
# test_dynamic_load.o: test_dynamic_load.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # link test driver with embedded rpath
# test_dynamic_load: test_dynamic_load.o $(LIB)
# 	$(CXX) -rdynamic \
# 	        -o $@ test_dynamic_load.o \
# 	        -L. -lsimreg $(LDLIBS) $(RPATH)

# clean:
# 	rm -f $(OBJ) $(LIB) test_dynamic_load




###############################################################################################################

# Simulator/Makefile

# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon

# # for building libsimreg.so
# LDFLAGS_SO := -shared



##################


# CXX      := g++
# CXXFLAGS := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon

# # build libsimreg.so as a macOS dynamic library
# LDFLAGS_SO := -dynamiclib -install_name @rpath/libsimreg.so

# # for linking the test binary
# LDLIBS_TEST := -L. -lsimreg -ldl
# # on macOS clang/ld, use -export_dynamic to expose all symbols
# EXPORT_SYMS := -Wl,-export_dynamic
# # rpath so the loader always looks next to the exe for libsimreg.so
# RPATH      := -Wl,-rpath,@loader_path

# SRC := AlgorithmRegistrar.cpp   \
#        PlayerRegistration.cpp   \
#        TankAlgorithmRegistration.cpp \
#        GameManagerRegistration.cpp \
#        GameManagerRegistrar.cpp

# OBJ := $(SRC:.cpp=.o)
# LIB := libsimreg.so


# all: $(LIB) test_dynamic_load

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # build the registrar as a macOS dylib
# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS_SO) -o $@ $^
# # compile the test driver
# test_dynamic_load.o: test_dynamic_load.cpp AlgorithmRegistrar.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # # link test driver: export host symbols + embed rpath
# # test_dynamic_load: test_dynamic_load.o $(LIB)
# # 	$(CXX) -o $@ $< $(LDLIBS_TEST) $(EXPORT_SYMS) $(RPATH)

# # link test driver: link against libsimreg.so and embed rpath
# test_dynamic_load: test_dynamic_load.o $(LIB)
# 	$(CXX) -o $@ $< $(LDLIBS_TEST) -Wl,-rpath,@loader_path

# clean:
# 	rm -f $(OBJ) $(LIB) test_dynamic_load

# .PHONY: all clean



####################

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

# SRC             := AlgorithmRegistrar.cpp   \
#                    PlayerRegistration.cpp   \
#                    TankAlgorithmRegistration.cpp \
#                    GameManagerRegistration.cpp \
#                    GameManagerRegistrar.cpp

# OBJ             := $(SRC:.cpp=.o)
# LIB             := libsimreg.so

# TP_SRCS         := ThreadPool.cpp
# TP_OBJS         := ThreadPool.o

# all: $(LIB) test_dynamic_load simulator_315634022

# # compile .o from .cpp
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # build the registrar as a macOS dylib
# $(LIB): $(OBJ)
# 	$(CXX) $(LDFLAGS_SO) -o $@ $^

# ThreadPool.o: ThreadPool.cpp ThreadPool.hpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # compile the test driver
# test_dynamic_load.o: test_dynamic_load.cpp AlgorithmRegistrar.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # link test driver: link against libsimreg.so and embed rpath
# test_dynamic_load: test_dynamic_load.o $(LIB)
# 	$(CXX) -o $@ test_dynamic_load.o $(LDLIBS_TEST) $(RPATH)

# # compile the simulator driver
# main.o: main.cpp AlgorithmRegistrar.h
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # link simulator: export host symbols + embed rpath
# # simulator_315634022: main.o $(LIB)
# # 	$(CXX) $(EXPORT_SYMS) -o $@ main.o $(LDLIBS_TEST) $(RPATH)

# simulator_315634022: main.o $(LIB) ThreadPool.o
# 	$(CXX) $(EXPORT_SYMS) -o $@ main.o ThreadPool.o $(LDLIBS_TEST) $(RPATH)
# clean:
# 	rm -f $(OBJ) $(LIB) test_dynamic_load main.o simulator_315634022

# .PHONY: all clean




CXX             := g++
CXXFLAGS        := -std=c++17 -fPIC -g -O0 -I. -I../common -I../UserCommon

# build libsimreg.so as a macOS dynamic library
LDFLAGS_SO      := -dynamiclib -install_name @rpath/libsimreg.so

# for linking the test driver and simulator executable
LDLIBS_TEST     := -L. -lsimreg -ldl
# on macOS clang/ld, use -export_dynamic to expose all symbols
EXPORT_SYMS     := -Wl,-export_dynamic
# rpath so the loader always looks next to the exe for libsimreg.so
RPATH           := -Wl,-rpath,@loader_path

# registration glue
SRC             := AlgorithmRegistrar.cpp   \
                   PlayerRegistration.cpp   \
                   TankAlgorithmRegistration.cpp \
                   GameManagerRegistration.cpp \
                   GameManagerRegistrar.cpp
OBJ             := $(SRC:.cpp=.o)

LIB             := libsimreg.so

# thread pool
TP_SRCS         := ThreadPool.cpp
TP_OBJS         := ThreadPool.o

# argument parser
AP_SRCS         := ArgParser.cpp
AP_OBJS         := ArgParser.o

all: $(LIB) test_dynamic_load simulator_315634022

# generic rule for .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# build the registrar as a macOS dylib
$(LIB): $(OBJ)
	$(CXX) $(LDFLAGS_SO) -o $@ $^

# build the thread‐pool object
ThreadPool.o: ThreadPool.cpp ThreadPool.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# build the argument‐parser object
ArgParser.o: ArgParser.cpp ArgParser.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# compile the test driver
test_dynamic_load.o: test_dynamic_load.cpp AlgorithmRegistrar.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link test driver with libsimreg and embed rpath
test_dynamic_load: test_dynamic_load.o $(LIB)
	$(CXX) -o $@ test_dynamic_load.o $(LDLIBS_TEST) $(RPATH)

# compile the simulator driver
main.o: main.cpp ArgParser.hpp AlgorithmRegistrar.h GameManagerRegistrar.h ThreadPool.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# link simulator: include parser, threadpool, and registrar lib
simulator_315634022: main.o ArgParser.o ThreadPool.o $(LIB)
	$(CXX) $(EXPORT_SYMS) -o $@ main.o ArgParser.o ThreadPool.o $(LDLIBS_TEST) $(RPATH)

clean:
	rm -f $(OBJ) $(LIB) test_dynamic_load main.o ArgParser.o ThreadPool.o simulator_315634022

.PHONY: all clean
