# be careful and *always* use := instead =, otherwise variable is not immediately created
CPATHZMQ := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

# source directories
SRCDIRS += $(CPATHZMQ)/cpp_readline

# include flags
MY_CFLAGS += -I$(CPATHZMQ)/cpp_readline

# libraries
MY_LIBS += -lreadline

# always undefine the following variable
#undefine CPATHZMQ

# DO NOT DELETE
