# ImageEncoder
# Encodes png image data into C++ source code.

TARGET_APP=ImageEncoder
PROJECT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: $(PROJECT_DIR)/$(TARGET_APP)

$(PROJECT_DIR)/$(TARGET_APP) :
	@echo Compiling "$(TARGET_APP)"
	@$(CXX) -o $(PROJECT_DIR)/$(TARGET_APP) \
    $(shell pkg-config --cflags libpng) -O3 -flto -std=gnu++14 \
    -fvisibility=hidden $(shell pkg-config --libs libpng) \
    $(PROJECT_DIR)/ImageEncoder.cpp
