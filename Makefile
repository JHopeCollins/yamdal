#-------------------------------------
# USER INPUTS:

# directories
INCLDEDIR = inc/#			header files
SOURCEDIR = src/#			class / function source files
SCRIPTDIR = script/#		main() function source files
PROGRMDIR = progrm/#		executables

# Class / function definition source files
CSOURCE =

# main() function files
CSCRIPT = test.cpp

# compiler
CCMP = g++-10

# debug/optimisation flags
COPT = -ggdb3 -D_GLIBCXX_DEBUG -fsanitize=address,undefined -fno-omit-frame-pointer
#COPT = -O3# -ffast-math -DNDEBUG

# warnings
CWARN = -Wall -Wextra -Wpedantic -Wshadow# -Wconversion

# language specs
CSTD = -std=c++20 -fno-exceptions -fopenmp #-fconcepts-diagnostics-depth=2

# external libraries eg lapack, blas
LIBS =


#-------------------------------------
#  variable definitions

DIRS = $(INCLDEDIR) $(SOURCEDIR) $(SCRIPTDIR) $(PROGRMDIR)

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
	$(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE) -o $@ -c $<

## wraps all source object files into one
#all.o : $(SOURCEOBJ)
#	ld -r $^ -o $@

# each executable depends on its own object file, and all source objects

$(PROGRM) : $(PROGRMDIR)%.out : $(SCRIPTDIR)%.o $(SOURCEOBJ)
	$(CCMP) $(COPT) $(CSTD) $(CWARN) $(INCLDE) -o $@ $^ $(LIBS)


#-------------------------------------
# misc recipes

.PHONY : all clean echo obj mkdir names $(PNAMES)

# make <pname> will compile only the executable 'pname.out'
$(PNAMES) : % : $(PROGRMDIR)%.out

# make all executables
all: $(PROGRM)

# make source objects
obj: $(SOURCEOBJ)

# create required directories
mkdir:
	mkdir $(DIRS)

# print names of all executables to standard output
names:
	@for name in $(PNAMES); do echo $$name; done

# delete all non-source files
clean:
	rm -f $(OBJS) $(PROGRM)

