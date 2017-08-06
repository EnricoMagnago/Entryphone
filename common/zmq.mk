# be careful and *always* use := instead =, otherwise variable is not immediately created
CPATHZMQ := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

# source directories
SRCDIRS += $(CPATHZMQ)/zmq_layer

# include flags
MY_CFLAGS += -I$(CPATHZMQ)/zmq_layer

# libraries
MY_LIBS += -lzmq -lpthread

# always undefine the following variable
#undefine CPATHZMQ

# DO NOT DELETE
