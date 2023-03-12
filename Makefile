TARGET = cachesim

# the root dir of *.o *.d
ODIR = build/Release

# the root dir of all source files
SDIR = src

# the relative dir path of replacement policy source files
RDIR = replace

# the relative dir path of admission policy source files
ADIR = admit

# the relative dir path of partition policy source files
PDIR = partition


OBJS = \
	$(ODIR)/$(RDIR)/lru.o \
	$(ODIR)/$(RDIR)/fifo.o \
	$(ODIR)/$(RDIR)/random.o \
	$(ODIR)/$(RDIR)/s4lru.o \
	$(ODIR)/$(RDIR)/arc.o \
	$(ODIR)/$(RDIR)/lirs.o \
	$(ODIR)/$(RDIR)/dlirs.o \
	$(ODIR)/$(RDIR)/min.o \
	$(ODIR)/cachesim.o \

LIBS += -lm

CXX = g++ #clang++ #OSX
CXXFLAGS += -std=c++14 #-stdlib=libc++ #non-linux
CXXFLAGS += -MMD -MP # dependency tracking flags
CXXFLAGS += -I./include
CXXFLAGS += -Wall -Werror 
LDFLAGS += $(LIBS)
all: CXXFLAGS += -O2 # release flags
all:		$(TARGET)

debug: CXXFLAGS += -ggdb  -D_GLIBCXX_DEBUG # debug flags
debug: $(TARGET)

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# compile cachesim
$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# compile replacement policy
$(ODIR)/$(RDIR)/%.o: $(SDIR)/$(RDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# compile admission policy
$(ODIR)/$(ADIR)/%.o: $(SDIR)/$(ADIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# compile partition policy
$(ODIR)/$(PDIR)/%.o: $(SDIR)/$(PDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

DEPS = $(OBJS:%.o=%.d)
-include $(DEPS)

clean:
	rm $(TARGET) $(OBJS) $(DEPS)