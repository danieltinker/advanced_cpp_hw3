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
