TARGET = bin/async_connect
SRCS = src/test/async_connect.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -pthread -lrt
DEPFILE = build/libbrickred.a
BUILD_DIR = build

include mak/main.mak

