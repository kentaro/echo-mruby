CC=gcc
EM_ROOT=$(shell pwd)
EM_BUILD_DIR=$(EM_ROOT)/build
EM_LIB_DIR=$(EM_BUILD_DIR)/lib
EM_INCLUDE_DIR=$(EM_BUILD_DIR)/include
EM_BIN_DIR=$(EM_BUILD_DIR)

MRUBY_MAK_FILE := $(EM_ROOT)/vendor/mruby/build/host/lib/libmruby.flags.mak
ifeq ($(wildcard $(MRUBY_MAK_FILE)),)
  MRUBY_CFLAGS =
  MRUBY_LDFLAGS =
  MRUBY_LIBS =
  MRUBY_LDFLAGS_BEFORE_LIBS =
else
  include $(MRUBY_MAK_FILE)
endif

EM_LIBS=-lmruby -lm $(MRUBY_LIBS) -lstdc++
EM_CFLAGS=-g -O0 -Iinclude -I$(EM_INCLUDE_DIR)
EM_LDFLAGS=-L$(EM_LIB_DIR) $(MRUBY_LDFLAGS)
EM_BIN=$(EM_BIN_DIR)/echo-mruby
EM_SRC=\
src/em.c \
src/em_server.c

#   the default target
all: em-all

#   compile binary
em-all: mruby
	$(CC) $(EM_CFLAGS) $(EM_LDFLAGS) $(EM_SRC) $(MRUBY_LDFLAGS_BEFORE_LIBS) $(EM_LIBS) -o $(EM_BIN)

#    compile mruby
mruby:
	test -f $(EM_BUILD_DIR)/lib/libmruby.a || (cd vendor/mruby && \
		make && mkdir -p $(EM_BUILD_DIR)/lib $(EM_BUILD_DIR)/include && \
    cp build/host/lib/libmruby.a $(EM_BUILD_DIR)/lib/. && \
		cp -r include/* $(EM_BUILD_DIR)/include/.)

#   clean
clean:
	-rm -rf $(EM_BUILD_DIR)/*
	cd vendor/mruby && rake deep_clean

.PHONY: all