TARGET = bin/echo_client
SRCS = src/test/echo_client.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -pthread -lrt
DEPFILE = build/libbrickred.a
BUILD_DIR = build

include mak/main.mak

