# FILEHEADER

SHELL = /bin/sh
BUILD_DIR := build
CC = cc
CFLAGS = -g
LD = ${CC}
LDFLAGS = 
DEMO_CRT_OBJS=demo_crt.o warpwar_crt.o
DEMO_SHIP_OBJS=demo_ship.o warpwar_ship.o warpwar_crt.o warpwar_combat.o
.SUFFIXES:
.SUFFIXES: .c .o

# c source
#$(BUILD_DIR)/%.c.o: %.c
#	$(MKDIR_P) $(dir $@)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all:	demo_crt demo_ship

demo_crt:	${DEMO_CRT_OBJS}
	${LD} $^ -o $@

demo_ship:	${DEMO_SHIP_OBJS}
	${LD} $^ -o $@

.PHONY: clean
clean:
	${RM} ${DEMO_CRT_OBJS} ${DEMO_SHIP_OBJS}

.PHONY: distclean
distclean:	clean
	${RM} demo_crt demo_ship
