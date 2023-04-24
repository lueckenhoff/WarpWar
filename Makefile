# FILEHEADER

SHELL = /bin/sh
BUILD_DIR := build
CC = cc
CFLAGS = -g
LD = ${CC}
LDFLAGS = 
CRT_DEMO_OBJS=crt_demo.o warpwar_crt.o
SHIP_DEMO_OBJS=ship_demo.o warpwar_ship.o
.SUFFIXES:
.SUFFIXES: .c .o

# c source
#$(BUILD_DIR)/%.c.o: %.c
#	$(MKDIR_P) $(dir $@)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:	crt_demo ship_demo

crt_demo:	${CRT_DEMO_OBJS}
	${LD} $^ -o $@

ship_demo:	${SHIP_DEMO_OBJS}
	${LD} $^ -o $@

.PHONY: clean
clean:
	${RM} ${CRT_DEMO_OBJS}
	${RM} ${SHIP_DEMO_OBJS}

.PHONY: distclean
distclean:	clean
	${RM} crt_demo
	${RM} ship_demo
