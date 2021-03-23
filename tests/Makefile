#-------------------------------------
# USER INPUTS:

# directories
targetINCLDEDIR = ../inc/#			header files to test

INCLDEDIR = inc/#			test header files
SOURCEDIR = src/#			test source files

SCRIPTDIR = script/#		test main() function source file
PROGRMDIR = progrm/#		test executables

# test source files
CSOURCE = index_h.cpp \
	external/mdspan_h.cpp \
	utility_h.cpp \
	span_h.cpp \
	views_h.cpp

# main() function file
CSCRIPT = tests.cpp

# compiler
#CCMP = g++-10
CCMP = clang++-10

# flags with different definitions for different compilers
ifeq ($(CCMP),clang++-10)
OPENMP_FLAG = -fopenmp=libomp
else ifeq ($(CCMP),g++-10)
OPENMP_FLAG = -fopenmp
else
$(error Only g++-10 or clang++-10 supported)
endif

# debug/optimisation flags
COPT = -ggdb3 -fno-omit-frame-pointer -fsanitize=address,undefined# -D_GLIBCXX_DEBUG
#COPT = -O3# -ffast-math -DNDEBUG

# warnings
CWARN = -Wall -Wextra -Wpedantic -Wshadow -Wconversion# -fconcepts-diagnostics-depth=2

# language specs
CSTD = -std=c++20 -fexceptions $(OPENMP_FLAG)

# external libraries eg lapack, blas
LIBS =


#-------------------------------------
#  variable definitions

DIRS = $(INCLDEDIR) $(SOURCEDIR) $(SCRIPTDIR) $(PROGRMDIR)

targetINCLDE = $(addprefix -I,$(targetINCLDEDIR))
INCLDE = $(addprefix -I,$(INCLDEDIR))

# full paths for source, script and executable files
SOURCE = $(addprefix $(SOURCEDIR),$(CSOURCE))
SCRIPT = $(addprefix $(SCRIPTDIR),$(CSCRIPT))
PROGRM = $(addprefix $(PROGRMDIR),$(CSCRIPT:.cpp=.out))

# name of program (no suffix)
PNAME = $(CSCRIPT:.cpp=)

# object files
SOURCEOBJ = $(SOURCE:.cpp=.o)
SCRIPTOBJ = $(SCRIPT:.cpp=.o)

OBJS = $(SOURCEOBJ) $(SCRIPTOBJ)

COMPILER_CMD = $(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE) $(targetINCLDE) 

#-------------------------------------
# compilation recipes

# default
%.o : %.cpp
	$(COMPILER_CMD) -o $@ -c $<

# executable depends on its own object file, and all source objects
$(PROGRM) : $(PROGRMDIR)%.out : $(SCRIPTDIR)%.o $(SOURCEOBJ)
	$(COMPILER_CMD) -o $@ $^ $(LIBS)


#-------------------------------------
# misc recipes

.PHONY : clean build obj clean-obj run

# make <pname> will compile only the executable 'pname.out'
build : $(PROGRM)

# make source objects
obj: $(SOURCEOBJ)

# delete all non-source files
clean:
	rm -f $(OBJS) $(PROGRM)

# delete source file objects except main object
clean-obj:
	rm -f $(SOURCEOBJ)

# run test
run: $(PROGRM)
	$(PROGRM) $(args)

