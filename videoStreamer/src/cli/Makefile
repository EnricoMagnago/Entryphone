# The linker options.
# libraries
MY_LIBS := `pkg-config --cflags --libs opencv` -pthread #-lm

# The options used in linking as well as in any direct use of ld.
LDFLAGS :=

# source directories
SRCDIRS += ./src

# Flags
CXXFLAGS = -std=c++11 -O3 -pedantic -pedantic-errors -Wall -Wextra
CFLAGS   = -  std=c11 -O3 -pedantic -pedantic-errors -Wall -Wextra

# The pre-processor options used by the cpp (man cpp for more).
CPPFLAGS  := -Wall -O3


# The executable file name.
# If not specified, current directory name or `a.out' will be used.
PROGRAM := video_streamer


# load common definitions from utils
#include ../../../common/standard.mk
# load zmq layer
include ../../../common/zmq.mk



###############################################
# ""AUTOMATIC"" PART

# The C++ program compiler.
#ifeq ($(CROSS_COMPILE_FLAG),on)
#	CXX=${CROSS_COMPILER}
#	CXXFLAGS += --sysroot=${SYST_ROOT}
#	CFLAGS   += --sysroot=${SYST_ROOT}
#else
	CXX = g++
#endif

# Verbose
#CXXFLAGS += -v
#CFLAGS += -v

# load common definitions to all makefile
include standard.mk
