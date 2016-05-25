# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# C source files included in this build.
CSRC +=

ifeq (,$(filter "PLATFORM_ID","103" "269"))
  SPARK_CPPSRC = $(call target_files,src/,spark_*.cpp)
  BLUZ_CPPSRC = $(call target_files,src/,bluz_*.cpp)
  CPPSRC += $(filter-out $(subst src/bluz_,src/spark_,$(BLUZ_CPPSRC)),$(SPARK_CPPSRC))
  CPPSRC += $(BLUZ_CPPSRC)
else
  CPPSRC += $(SPARK_CPPSRC)
endif

# ASM source files included in this build.
ASRC +=

CPPFLAGS += -std=gnu++11

BUILTINS_EXCLUDE = malloc free realloc
CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

