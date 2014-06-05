TARGET = bin/sha1_sum
SRCS = src/test/sha1_sum.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickredcore -lbrtest -pthread -lrt
DEPFILE = build/libbrickredcore.a build/libbrtest.a
BUILD_DIR = build

include mak/main.mak

