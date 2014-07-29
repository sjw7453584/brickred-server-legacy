TARGET = bin/echo_server2
SRCS = src/test/echo_server2.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickredcore -pthread -lrt
DEPFILE = build/libbrickredcore.a
BUILD_DIR = build

include mak/main.mak
