# Helpful macros
TARGET_EXEC ?= ./bin/edge-reverse
OBJ_DIR ?= ./objects
SRC_DIRS ?= ./source

# Macros for CC
CC = mpicc
CFLAGS = -Wall -Wpedantic -fopenmp
LIBS = -lm

# Directories
SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Include Macros
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Compile the source
$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $@

# Create object files
$(OBJ_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)/*

-include $(DEPS)

MKDIR_P ?= mkdir -p
