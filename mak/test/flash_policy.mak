TARGET = bin/flash_policy
SRCS = src/test/flash_policy.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -pthread -lrt
DEPFILE = build/libbrickred.a
BUILD_DIR = build

include mak/main.mak

