#-------------------------------------
# USER INPUTS:

# directories
INCLDEDIR = inc/#			header files
SCRIPTDIR = script/#		main() function source files
PROGRMDIR = progrm/#		executables

# main() function files
CSCRIPT = test.cpp \
	test_views.cpp \
	test_mdspan.cpp \
	test_reduce.cpp \
	test_array.cpp \
	extents_manip.cpp

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
CSTD = -std=c++20 -fno-exceptions $(OPENMP_FLAG)

# external libraries eg lapack, blas
LIBS =


#-------------------------------------
#  variable definitions

DIRS = $(INCLDEDIR) $(SCRIPTDIR) $(PROGRMDIR)

INCLDE = $(addprefix -I,$(INCLDEDIR))

# full paths for source, script and executable files
SCRIPT = $(addprefix $(SCRIPTDIR),$(CSCRIPT))
PROGRM = $(addprefix $(PROGRMDIR),$(CSCRIPT:.cpp=.out))

# names of programs (no suffix)
PNAMES = $(CSCRIPT:.cpp=)

# object files
SCRIPTOBJ = $(SCRIPT:.cpp=.o)

OBJS = $(SCRIPTOBJ)


COMPILER_CMD = $(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE)

#-------------------------------------
# compilation recipes

# default
%.o : %.cpp
	$(COMPILER_CMD) -o $@ -c $<

# each executable depends on its own object file, and all source objects
$(PROGRM) : $(PROGRMDIR)%.out : $(SCRIPTDIR)%.o
	$(COMPILER_CMD) -o $@ $^ $(LIBS)


#-------------------------------------
# misc recipes

.PHONY : clean $(PNAMES) all mkdir names

# delete all non-source files
clean:
	rm -f $(OBJS) $(PROGRM)

# make <pname> will compile only the executable 'pname.out'
$(PNAMES) : % : $(PROGRMDIR)%.out

# make all executables
all: $(PROGRM)

# create required directories
mkdir:
	mkdir $(DIRS)

# print names of all executables to standard output
names:
	@for name in $(PNAMES); do echo $$name; done

