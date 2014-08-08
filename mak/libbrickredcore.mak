include config.mak

TARGET = build/libbrickredcore
SRCS = src/brickred/base_logger.cc \
       src/brickred/command_line_option.cc \
       src/brickred/condition_variable.cc \
       src/brickred/dynamic_buffer.cc \
       src/brickred/socket_address.cc \
       src/brickred/io_device.cc \
       src/brickred/io_service.cc \
       src/brickred/log_core.cc \
       src/brickred/log_async_sink.cc \
       src/brickred/log_file_sink.cc \
       src/brickred/log_stderr_sink.cc \
       src/brickred/mutex.cc \
       src/brickred/random.cc \
       src/brickred/self_pipe.cc \
       src/brickred/tcp_socket.cc \
       src/brickred/tcp_service.cc \
       src/brickred/thread.cc \
       src/brickred/timer_heap.cc \
       src/brickred/timestamp.cc \
       src/brickred/string_util.cc \
       src/brickred/codec/base64.cc \
       src/brickred/codec/sha1.cc \
       src/brickred/protocol/http_message.cc \
       src/brickred/protocol/http_protocol.cc \
       src/brickred/protocol/http_request.cc \
       src/brickred/protocol/http_response.cc \
       src/brickred/protocol/web_socket_protocol.cc
LINK_TYPE = static
INCLUDE = -Isrc
CPP_FLAG = $(BRCORE_CPP_FLAG)
BUILD_DIR = build

include mak/main.mak

.PHONY: install

install:
	@mkdir -p $(BR_INSTALL_PREFIX)/include/brickred
	@mkdir -p $(BR_INSTALL_PREFIX)/include/brickred/protocol
	@mkdir -p $(BR_INSTALL_PREFIX)/include/brickred/codec
	@cp src/brickred/*.h $(BR_INSTALL_PREFIX)/include/brickred
	@cp src/brickred/protocol/*.h $(BR_INSTALL_PREFIX)/include/brickred/protocol
	@cp src/brickred/codec/*.h $(BR_INSTALL_PREFIX)/include/brickred/codec
	@mkdir -p $(BR_INSTALL_PREFIX)/lib
	@cp build/libbrickredcore.a $(BR_INSTALL_PREFIX)/lib
