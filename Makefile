
CXXFLAGS+= -g
CXXFLAGS     += `root-config --cflags` -fPIC
LDFLAGS      += `root-config --ldflags`
LIBS          = `root-config --glibs` -g -Wl,-z,defs -lMathMore -Wall
GLIBS         = 
LIBDIR=lib
BUILDDIR=build
INCLUDEDIR=include
BINDIR=bin

.PHONY: clean all


OBJS := $(addprefix $(BUILDDIR)/, BitsetMaker.o TriggerBits.o TriggerTestDict.o)
INCLUDES := $(addprefix $(INCLUDEDIR)/, $(shell ls $(INCLUDEDIR)))
BINARIES := $(addprefix $(BINDIR)/, doNoise) 

LINKLIBNAME=TriggerTest
LIBNAME = $(LIBDIR)/lib$(LINKLIBNAME).so

all: $(LIBNAME) $(BINARIES) 

### probably need some magic for Mac OS X here? 
$(LIBNAME): $(OBJS) | $(LIBDIR)
	@echo Building shared library $@
	@$(CXX) $(SOFLAGS) $(LDFLAGS) $(OBJS) -shared -o $@ $(LIBS) $(GLIBS) 
	cp $(BUILDDIR)/*.pcm $(LIBDIR) 


$(OBJS): | $(BUILDDIR)

$(BUILDDIR): 
	mkdir -p $(BUILDDIR)

$(BINDIR): 
	mkdir -p $(BINDIR)

$(LIBDIR): 
	mkdir -p $(LIBDIR)


$(BUILDDIR)/%.o: src/%.cc $(INCLUDES) Makefile | $(BUILDDIR) $(VECTORIZE)
	@echo Compiling  $< 
	@$(CXX)  -I./include $(CXXFLAGS) -o $@ -c $< 

$(BUILDDIR)/%.o: build/%.cc $(INCLUDES) Makefile | $(BUILDDIR) 
	@echo Compiling  $< 
	$(CXX)  -I../include -I./ $(CXXFLAGS) -o $@ -c $< 

$(BINDIR)/%: src/%.cc $(INCLUDES) Makefile $(LIBNAME) | $(BINDIR)
	@echo Compiling $<
	$(CXX)  -I./include -I./ $(CXXFLAGS) -o $@   $<  $(LDFLAGS)  $(LIBS) -L./$(LIBDIR) -l$(LINKLIBNAME)

$(BUILDDIR)/TriggerTestDict.cc: $(INCLUDES) LinkDef.h | $(BUILDDIR)
	@echo Running rootcint
	rootcint  -f $@ -c -p -I$(ANITA_UTIL_INSTALL_DIR)/include $(INCLUDES) LinkDef.h


clean: 
	rm -rf build
	rm -rf bin
	rm -rf lib
