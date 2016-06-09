# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# C source files included in this build.
CSRC +=

SPARK_CPPSRC = $(call target_files,$(shell find src ! -name bluz_*.cpp -name *.cpp))
ifeq ($(PLATFORM_ID),$(filter $(PLATFORM_ID),103 269))
  # replace any spark_.cpp files with bluz_*.cpp versions, if present
  BLUZ_CPPSRC = $(call target_files,src/,bluz_*.cpp)
  CPPSRC += $(filter-out $(subst src/bluz_,src/spark_,$(BLUZ_CPPSRC)),$(SPARK_CPPSRC))
  CPPSRC += $(BLUZ_CPPSRC)
else
  CPPSRC += $(SPARK_CPPSRC)
endif
#$(info AAA_DEBUG_AAA SPARK_CPPSRC=$(SPARK_CPPSRC))
#$(info XXX_DEBUG_XXX CPPSRC=$(CPPSRC))

# ASM source files included in this build.
ASRC +=

CPPFLAGS += -std=gnu++11

BUILTINS_EXCLUDE = malloc free realloc
CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

