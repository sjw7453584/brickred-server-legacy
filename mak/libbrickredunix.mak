include config.mak

TARGET = build/libbrickredunix
SRCS = \
src/brickred/unix/signal_queue.cc \
src/brickred/unix/system.cc \

LINK_TYPE = static
INCLUDE = -Isrc
BUILD_DIR = build

include mak/main.mak

.PHONY: install

install:
	@mkdir -p $(BR_INSTALL_PREFIX)/include/brickred/unix
	@cp src/brickred/unix/*.h $(BR_INSTALL_PREFIX)/include/brickred/unix
	@mkdir -p $(BR_INSTALL_PREFIX)/lib
	@cp $(FINAL_TARGET) $(BR_INSTALL_PREFIX)/lib
