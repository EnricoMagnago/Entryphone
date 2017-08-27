# PER ORA LANCIARE I COMANDI QUI SOTTO PRIMA DI FARE IL MAKE
#export SYSROOT=/home/valerio/Documents/CrossCompiler/sysRootBBB_walker_16-08-2017
#export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}
#export PKG_CONFIG_DIR=${SYSROOT}
#export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/local/lib/pkgconfig



#Cross compiler OPTION
#CROSS_COMPILE_FLAG=on   #Enable CrossCompiler
#CROSS_COMPILE_FLAG=off  #Disable Cross Compiler

# Sysroot of the beaglebone
#SYST_ROOT="/home/valerio/Documents/CrossCompiler/sysRootBBB_walker_16-08-2017"


# Location of the cross compiler
#CROSS_COMPILER="/home/valerio/Documents/CrossCompiler/gcc-linaro-5.3.1-2016.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++"

# The pre-processor and compiler options.
#MY_CFLAGS :=  -I/home/valerio/Documents/CrossCompiler/sysRootBBB_walker_16-08-2017/usr/include/opencv2

# The linker options.
# libraries
MY_LIBS := -lraspicam -pthread

#MY_CFLAGS :=
# IF plot debug enabled
#makMY_LIBS   += -lpython2.7
#MY_CFLAGS += -I/usr/include/python2.7

# The options used in linking as well as in any direct use of ld.
LDFLAGS :=

# source directories
SRCDIRS += ./src
SRCDIRS += /usr/local/lib

# Flags
CXXFLAGS = -std=c++11 -O3 -pedantic -pedantic-errors -Wall -Wextra -L /usr/local/lib
CFLAGS   = -std=c11 -O3 -pedantic -pedantic-errors -Wall -Wextra -L /usr/local/lib

# The pre-processor options used by the cpp (man cpp for more).
CPPFLAGS  := -Wall -O3


# The executable file name.
# If not specified, current directory name or `a.out' will be used.
PROGRAM := video_streamer


# load common definitions from common
#include ../common/standard.mk
# load zmq layer
include ../common/zmq.mk



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
