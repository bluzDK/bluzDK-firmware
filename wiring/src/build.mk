# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory.
# Note: The name must be unique within this build and should be
#       based on the root of the project
TARGET_SRC_PATH = src

# C source files included in this build.
CSRC +=

# enumerate target cpp files into temporary variable, skipping potential bluz_*.cpp files
CPPSRC_T = $(call target_files,src/,spark_*.cpp) # NOTE: spark_*.cpp (was *.cpp)

# If we building the Bluz platform ...
# replace specific spark_wiring sources with bluz_wiring sources.
# We do this to avoid lot of #ifdefs over large code block in the individual files
ifeq ("PLATFORM_ID","103")
else ("PLATFORM_ID","269") # this is how we OR in gnu make
  PLATFORM_BLUZ = true
endif
ifeq ($(PLATFORM_BLUZ),true)
  # change out each file as required
  CPPSRC += $(CPPSRC_T:src/spark_wiring_timer.cpp=src/bluz_wiring_timer.cpp) 
# maybe later...
# CPPSRC += $(CPPSRC_T:src/spark_wiring_tcpclient.cpp=src/bluz_wiring_tcpclient.cpp) 
else
  # not building Bluz, so run with the original CPPSRC list
  CPPSRC += $(CPPSRC_T)
endif

# ASM source files included in this build.
ASRC +=

CPPFLAGS += -std=gnu++11

BUILTINS_EXCLUDE = malloc free realloc
CFLAGS += $(addprefix -fno-builtin-,$(BUILTINS_EXCLUDE))

