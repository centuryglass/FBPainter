define FBPAINTER_HELPTEXT
## FBPainter Makefile ##
# This makefile provides the fbpainter build target to compile all FBPainter
# objects, and defines the $(FBPAINTER_OBJECTS) variable used when linking those
# objects.
#
## Quick Guide: ##
# 1. Ensure the following variables are defined when building the library:
#    - FBP_OBJDIR
#
# 2. Optionally, provide valid definitions for these additional variables to
#    enable features or override default values:
#    - FBP_ENABLE_LIBPNG
#    - FBP_CONFIG
#    - FBP_VERBOSE
#    - FBP_STRIP
#    - FBP_TARGET_ARCH
# 
# 3. Optionally, provide valid definitions for any of these variables to add new
#    build arguments:
#    - FBP_CFLAGS
#    - FBP_CXXFLAGS
#    - FBP_INCLUDE_FLAGS
#    - FBP_DEFINE_FLAGS
#    - FBP_CPPFLAGS
#
# 3. Include this makefile in your program's makefile.
#
# 4. Add fbpainter as a dependency of your program's main build target.
#
# 4. When linking your program, add $(FBPAINTER_OBJECTS) to your list of object
#    file arguments.
#
endef
export HELPTEXT


###################### Initialize project variables: ##########################
# Build type: either Debug or Release
FBP_CONFIG?=Release
# Enable or disable verbose output
FBP_VERBOSE?=0
FBP_V_AT:=$(shell if [ $(FBP_VERBOSE) != 1 ]; then echo '@'; fi)
# Enable or disable libpng support
FBP_ENABLE_LIBPNG?=1

# Project directories:
FBP_PROJECT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
FBP_SOURCE_DIR:=$(FBP_PROJECT_DIR)/Source

# Command used to strip unneeded symbols from object files:
FBP_STRIP?=strip

# Use the build system's architecture by default.
FBP_TARGET_ARCH?=-march=native

# Command used to clean out build files:
FBP_CLEANCMD:=rm -rf $(FBP_OBJDIR)

############################### Set build flags: ##############################
#### Config-specific flags: ####
ifeq ($(FBP_CONFIG),Debug)
    FBP_OPTIMIZATION?=1
    FBP_GDB_SUPPORT?=0
    # Debug-specific preprocessor definitions:
    FBP_CONFIG_FLAGS=-DFBP_DEBUG=1
endif

ifeq ($(FBP_CONFIG),Release)
    FBP_OPTIMIZATION?=1
    FBP_GDB_SUPPORT?=0
endif

# Set optimization level flags:
ifeq ($(FBP_OPTIMIZATION),1)
    FBP_CONFIG_CFLAGS=-O3 -flto
    FBP_CONFIG_LDFLAGS:=-flto
else
    FBP_CONFIG_CFLAGS=-O0
endif

# Set debugger flags:
ifeq ($(FBP_GDB_SUPPORT),1)
    FBP_CONFIG_CFLAGS:=$(FBP_CONFIG_CFLAGS) -g -ggdb
else
    FBP_CONFIG_LDFLAGS:=$(FBP_CONFIG_LDFLAGS) -fvisibility=hidden
endif

#### C compilation flags: ####
FBP_CFLAGS:=$(FBP_TARGET_ARCH) $(FBP_CONFIG_CFLAGS) $(FBP_CFLAGS)

#### C++ compilation flags: ####
FBP_CXXFLAGS:=-std=gnu++14 $(FBP_CXXFLAGS)

#### C Preprocessor flags: ####

# Include directories:
FBP_INCLUDE_FLAGS:=-I$(FBP_SOURCE_DIR) $(FBP_INCLUDE_FLAGS)

# Disable dependency generation if multiple architectures are set
FBP_DEPFLAGS:=$(if $(word 2, $(FBP_TARGET_ARCH)), , -MMD)

FBP_DEFINE_FLAGS:=-DUSE_PNG=$(FBP_ENABLE_LIBPNG) $(FBP_DEFINE_FLAGS)

FBP_CPPFLAGS:=-pthread \
          $(FBP_DEPFLAGS) \
          $(FBP_CONFIG_FLAGS) \
          $(FBP_DEFINE_FLAGS) \
          $(FBP_INCLUDE_FLAGS) \
          $(FBP_CPPFLAGS)

#### Aggregated build arguments: ####

FBPAINTER_OBJECTS:=$(FBP_OBJDIR)/FrameBuffer.o \
                   $(FBP_OBJDIR)/ImagePainter.o \
                   $(FBP_OBJDIR)/RGBPixel.o \
                   $(FBP_OBJDIR)/RGBAPixel.o

ifeq ($(FBP_ENABLE_LIBPNG),1)
    FBPAINTER_OBJECTS:=$(FBP_OBJDIR)/PngImage.o $(FBPAINTER_OBJECTS)
endif

# Complete set of flags used to compile source files:
FBP_BUILD_FLAGS:=$(FBP_CFLAGS) $(FBP_CXXFLAGS) $(FBP_CPPFLAGS)

###################### Supporting Build Targets: ##############################
.PHONY: fbpainter

fbpainter : $(FBPAINTER_OBJECTS)

$(FBPAINTER_OBJECTS) :
	@echo "Compiling $(<F):"
	$(FBP_V_AT)mkdir -p $(FBP_OBJDIR)
	$(FBP_V_AT)$(CXX) $(FBP_BUILD_FLAGS) -o "$@" -c "$<"

-include $(FBPAINTER_OBJECTS:%.o=%.d)

$(FBP_OBJDIR)/FrameBuffer.o: \
	$(FBP_SOURCE_DIR)/FrameBuffer.cpp
$(FBP_OBJDIR)/ImagePainter.o: \
	$(FBP_SOURCE_DIR)/ImagePainter.cpp
$(FBP_OBJDIR)/RGBPixel.o: \
	$(FBP_SOURCE_DIR)/RGBPixel.cpp
$(FBP_OBJDIR)/RGBAPixel.o: \
	$(FBP_SOURCE_DIR)/RGBAPixel.cpp
$(FBP_OBJDIR)/PngImage.o: \
	$(FBP_SOURCE_DIR)/PngImage.cpp
