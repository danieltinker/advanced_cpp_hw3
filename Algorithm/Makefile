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
CXX       := g++
CXXFLAGS  := -fPIC -std=c++17 -I../common -I../UserCommon
LDFLAGS   := -shared
LIBS      := -L../Simulator -lsimreg

# output directory for .so files
LIBDIR    := sos

# object sets for each plugin
ALGO1_OBJS := TankAlgorithm_315634022.o Player_315634022.o
ALGO2_OBJS := TankAlgorithmAlt_315634022.o PlayerAlt_315634022.o

all: $(LIBDIR) \
     $(LIBDIR)/libAlgorithm_315634022.so \
     $(LIBDIR)/libAlgorithmAlt_315634022.so

# ensure the output directory exists
$(LIBDIR):
	mkdir -p $@

# first algorithm .so
$(LIBDIR)/libAlgorithm_315634022.so: $(ALGO1_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# second algorithm .so
$(LIBDIR)/libAlgorithmAlt_315634022.so: $(ALGO2_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

# generic rule for all .cpp -> .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.so $(LIBDIR)

.PHONY: all clean

