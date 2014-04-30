TARGET = bin/ws_echo_client
SRCS = src/test/ws_echo_client.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -lbrtest -pthread -lrt
DEPFILE = build/libbrickred.a build/libbrtest.a
BUILD_DIR = build

include mak/main.mak

