TARGET = bin/testsocket
SRCS = src/test/test_socket.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -pthread -lrt
DEPFILE = build/libbrickred.a
BUILD_DIR = build

include mak/main.mak

