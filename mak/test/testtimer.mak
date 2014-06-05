TARGET = bin/testtimer
SRCS = src/test/test_timer.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickredcore -pthread -lrt
DEPFILE = build/libbrickredcore.a
BUILD_DIR = build

include mak/main.mak

