CC=gcc
EMS_ROOT=$(shell pwd)
EMS_BUILD_DIR=$(EMS_ROOT)/build
EMS_LIB_DIR=$(EMS_BUILD_DIR)/lib
EMS_INCLUDE_DIR=$(EMS_BUILD_DIR)/include
EMS_BIN_DIR=$(EMS_BUILD_DIR)

MRUBY_MAK_FILE := $(EMS_ROOT)/vendor/mruby/build/host/lib/libmruby.flags.mak
ifeq ($(wildcard $(MRUBY_MAK_FILE)),)
  MRUBY_CFLAGS =
  MRUBY_LDFLAGS =
  MRUBY_LIBS =
  MRUBY_LDFLAGS_BEFORE_LIBS =
else
  include $(MRUBY_MAK_FILE)
endif

EMS_LIBS=-lmruby -lm $(MRUBY_LIBS) -lstdc++
EMS_CFLAGS=-g -O0 -Iinclude -I$(EMS_INCLUDE_DIR)
EMS_LDFLAGS=-L$(EMS_LIB_DIR) $(MRUBY_LDFLAGS)
EMS_BIN=$(EMS_BIN_DIR)/ems_server
EMS_SRC=\
src/ems.c \
src/ems_server.c

#   the default target
all: ems-all

#   compile binary
ems-all: mruby
	$(CC) $(EMS_CFLAGS) $(EMS_LDFLAGS) $(EMS_SRC) $(MRUBY_LDFLAGS_BEFORE_LIBS) $(EMS_LIBS) -o $(EMS_BIN)

#    compile mruby
mruby:
	test -f $(EMS_BUILD_DIR)/lib/libmruby.a || (cd vendor/mruby && \
		make && mkdir -p $(EMS_BUILD_DIR)/lib $(EMS_BUILD_DIR)/include && \
    cp build/host/lib/libmruby.a $(EMS_BUILD_DIR)/lib/. && \
		cp -r include/* $(EMS_BUILD_DIR)/include/.)

#   run
run:
	./build/ems_server

#   clean
clean:
	-rm -rf $(EMS_BUILD_DIR)/*
	cd vendor/mruby && rake deep_clean

.PHONY: all