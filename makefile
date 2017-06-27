# ----------------------------------------------------
# Makefile
# ----------------------------------------------------
 
# General
CC        = gcc
SHELL     = /bin/sh
MKDIR_P  ?= mkdir -p

# Installing
INST_DIR  = /usr/bin

# Project
TARGET   ?= hex_to_256

BLD_DIR   = ./build
SRC_DIR   = ./src

INC_DIRS := 
LIB_DIRS  = 

SOURCES  := $(shell find $(SRC_DIR) -name '*.c')
HEADERS  := $(shell find $(SRC_DIR) -name '*.h')
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(BLD_DIR)/%.o)

MANUAL    = readme.md

# Flags
IFLAGS  := $(addprefix -I,$(INC_DIRS)) 
LFLAGS  := $(addprefix -L,$(LIB_DIRS))

CFLAGS   := -std=c99 -W -g -c $(IFLAGS) 
LDFLAGS   = $(LFLAGS) 

# ----------------------------------------------------

quiet_cmd_cc    = "  CC      $@"
quiet_cmd_ld    = "  LD      $@"
quiet_cmd_clean = "  CLEAN   $(BLD_DIR)"
quiet_cmd_exe   = "  EXE     $(BLD_DIR)/$(TARGET)"
quiet_cmd_inst  = "  INSTALL $(INST_DIR)"
quiet_cmd_uinst = "  REMOVE  $(INST_DIR)/$(TARGET)"
quiet_cmd_ar    = "  AR      $(@)"

# ----------------------------------------------------
 
default: compile

clean:
	@echo $(quiet_cmd_clean)
	@rm -r $(BLD_DIR)

install: compile
	@echo $(quiet_cmd_inst)
	@echo "$(INST_DIR)/$(TARGET)" > ./install.manifest
	@cp $(BLD_DIR)/$(TARGET) $(INST_DIR)

uninstall:
	@echo $(quiet_cmd_uinst)
	@rm $(INST_DIR)/$(TARGET)

lib: $(BLD_DIR)/lib$(TARGET).a
	@$(MKDIR_P) $(BLD_DIR)/include/
	@cp $(HEADERS) $(BLD_DIR)/include/

options:
	@echo "Build options for [$(TARGET)]:"
	@echo "Source Directory : $(SRC_DIR)"
	@echo "Build Directory  : $(BLD_DIR)"
	@echo "CFLAGS           : $(CFLAGS)"
	@echo "LDFLAGS          : $(LDFLAGS)"
	@echo ""

compile: $(BLD_DIR)/$(TARGET)

run: compile
	@echo $(quiet_cmd_exe)
	@$(BLD_DIR)/$(TARGET)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $(quiet_cmd_cc)
	@$(MKDIR_P) $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BLD_DIR)/$(TARGET): $(OBJECTS) 
	@echo $(quiet_cmd_ld)
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BLD_DIR)/lib$(TARGET).a: $(OBJECTS) 
	@echo $(quiet_cmd_ar)
	@ar rs $(BLD_DIR)/lib$(TARGET).a $(OBJECTS)

