# ----------------------------------------------------
# Default makefile
# ----------------------------------------------------
TARGET  = hex_to_256

# Init
SHELL = /bin/sh
CC = gcc

# Flags
FLAGS   		= -std=c99 $(INCLUDE_DIRS)
CFLAGS 			= -W -g
LDFLAGS 		=
DEBUGFLAGS   	= -O0 -D _DEBUG
DISTFLAGS    	= -O2 -D NDEBUG -combine -fwhole-program
TESTFLAGS 		= 
LIBS 			=
INCLUDE_DIRS 	= 

# Files
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:.c=.o)

# Default
default: compile

# Compile
compile: $(TARGET)

# Doc
doc: clean
	doxygen Doxyfile

install: compile
	cp ./$(TARGET) /usr/bin

uninstall:
	rm /usr/bin/$(TARGET)

# Clean
clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)

options:
	@echo "$(TARGET) build options:"
	@echo "FLAGS        = ${FLAGS}"
	@echo "CFLAGS       = ${CFLAGS}"
	@echo "LDFLAGS      = ${LDFLAGS}"
	@echo "DEBUGFLAGS   = ${DEBUGFLAGS}"
	@echo "RELEASEFLAGS = ${RELEASEFLAGS}"
	@echo "INCLUDE_DIRS = ${INCLUDE_DIRS}"
	@echo "EXTRA_DIST   = ${EXTRA_DIST}"
	@echo
	@echo "DEBUG: ${DEBUG}"
	@echo "DIST: ${DIST}"
	@echo "TEST: ${TEST}"

# Compile AUX
$(TARGET): $(OBJECTS) 
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -c -o $@ $<




