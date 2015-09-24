# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# C source files included in this build.
CSRC +=

# enumerate target cpp files
CPPSRC += $(call target_files,src/,*.cpp)

#ifdef BLUZ
$(call (echo $CPPSRC))
file := src/main.cpp
empty :=
CPPSRC := $(subst $(file),$(empty),$(CPPSRC))
#endif

# ASM source files included in this build.
ASRC +=

CPPFLAGS += -std=gnu++11
