TARGET = bin/dns_query
SRCS = src/test/dns_query.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickred -lbrtest -pthread -lrt
DEPFILE = build/libbrickred.a build/libbrtest.a
BUILD_DIR = build

include mak/main.mak

