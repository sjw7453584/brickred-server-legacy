TARGET = bin/url_encode
SRCS = src/test/url_encode.cc
LINK_TYPE = exec
INCLUDE = -Isrc
LIB = -Lbuild -lbrickredcore -lbrtest -pthread -lrt
DEPFILE = build/libbrickredcore.a build/libbrtest.a
BUILD_DIR = build

include mak/main.mak
