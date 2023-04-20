#!/bin/sh -x
CC=cc
CFLAGS=
LD=$CC

$CC $CFLAGS -c crt_demo.c 
$CC $CFLAGS -c warpwar_crt.c
$LD crt_demo.o warpwar_crt.o -o crt_demo
