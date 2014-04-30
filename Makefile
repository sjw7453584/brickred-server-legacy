MAKE = make --no-print-directory

define ECHO
	@printf "\033[;31m"; printf $1; printf "\033[0m\n"
endef

debug release clean profile:
	@$(call ECHO, "[build libbrickred]")
	@$(MAKE) -f mak/libbrickred.mak $@
	@$(call ECHO, "[build libbrtest]")
	@$(MAKE) -f mak/test/libbrtest.mak $@
	@$(call ECHO, "[build testcodec]")
	@$(MAKE) -f mak/test/testcodec.mak $@
	@$(call ECHO, "[build teststringutil]")
	@$(MAKE) -f mak/test/teststringutil.mak $@
	@$(call ECHO, "[build testlog]")
	@$(MAKE) -f mak/test/testlog.mak $@
	@$(call ECHO, "[build testrandom]")
	@$(MAKE) -f mak/test/testrandom.mak $@
	@$(call ECHO, "[build testsocket]")
	@$(MAKE) -f mak/test/testsocket.mak $@
	@$(call ECHO, "[build testsocket2]")
	@$(MAKE) -f mak/test/testsocket2.mak $@
	@$(call ECHO, "[build testtimer]")
	@$(MAKE) -f mak/test/testtimer.mak $@
	@$(call ECHO, "[build async_connect]")
	@$(MAKE) -f mak/test/async_connect.mak $@
	@$(call ECHO, "[build base64_encode]")
	@$(MAKE) -f mak/test/base64_encode.mak $@
	@$(call ECHO, "[build base64_decode]")
	@$(MAKE) -f mak/test/base64_decode.mak $@
	@$(call ECHO, "[build dns_query]")
	@$(MAKE) -f mak/test/dns_query.mak $@
	@$(call ECHO, "[build echo_server]")
	@$(MAKE) -f mak/test/echo_server.mak $@
	@$(call ECHO, "[build echo_client]")
	@$(MAKE) -f mak/test/echo_client.mak $@
	@$(call ECHO, "[build flash_policy]")
	@$(MAKE) -f mak/test/flash_policy.mak $@
	@$(call ECHO, "[build http_client]")
	@$(MAKE) -f mak/test/http_client.mak $@
	@$(call ECHO, "[build http_server]")
	@$(MAKE) -f mak/test/http_server.mak $@
	@$(call ECHO, "[build sha1_sum]")
	@$(MAKE) -f mak/test/sha1_sum.mak $@
	@$(call ECHO, "[build sha1_sum_binary]")
	@$(MAKE) -f mak/test/sha1_sum_binary.mak $@
	@$(call ECHO, "[build ws_echo_client]")
	@$(MAKE) -f mak/test/ws_echo_client.mak $@
	@$(call ECHO, "[build ws_echo_server]")
	@$(MAKE) -f mak/test/ws_echo_server.mak $@

