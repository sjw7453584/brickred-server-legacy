TARGET = bin/sha1_sum_binary
SRCS = src/test/sha1_sum_binary.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickredcore -lbrtest -pthread -lrt
DEPFILE = build/libbrickredcore.a build/libbrtest.a
BUILD_DIR = build

include mak/main.mak

