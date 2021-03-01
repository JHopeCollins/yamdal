#-------------------------------------
# USER INPUTS:

# directories
targetINCLDEDIR = ../inc/#			header files to test

INCLDEDIR = inc/#			test header files
SOURCEDIR = src/#			test source files

SCRIPTDIR = script/#		test main() function source file
PROGRMDIR = progrm/#		test executables

# test source files
CSOURCE = test_index_h.cpp \
	test_external_mdspan_h.cpp \
	test_utility_h.cpp

# main() function files
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

# names of programs (no suffix)
PNAMES = $(CSCRIPT:.cpp=)

# object files
SOURCEOBJ = $(SOURCE:.cpp=.o)
SCRIPTOBJ = $(SCRIPT:.cpp=.o)

OBJS = $(SOURCEOBJ) $(SCRIPTOBJ)


#-------------------------------------
# compilation recipes

# default
%.o : %.cpp
	$(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE) $(targetINCLDE) -o $@ -c $<

# each executable depends on its own object file, and all source objects
$(PROGRM) : $(PROGRMDIR)%.out : $(SCRIPTDIR)%.o $(SOURCEOBJ)
	$(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE) $(targetINCLDE) -o $@ $^ $(LIBS)


#-------------------------------------
# misc recipes

.PHONY : clean $(PNAMES) all obj clean-obj mkdir names

# delete all non-source files
clean:
	rm -f $(OBJS) $(PROGRM)

# make <pname> will compile only the executable 'pname.out'
$(PNAMES) : % : $(PROGRMDIR)%.out

# make all executables
all: $(PROGRM)

# make source objects
obj: $(SOURCEOBJ)

# delete source file objects except main object
clean-obj:
	rm -f $(SOURCEOBJ)

# create required directories
mkdir:
	mkdir $(DIRS)

# print names of all executables to standard output
names:
	@for name in $(PNAMES); do echo $$name; done

