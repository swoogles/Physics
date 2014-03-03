################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include $(DEPFILES)

RM := rm -rf

CC=g++

USER_OBJS :=

# 32 Bit/Laptop version
OPENGL_LIBS := -lGL -lGLU -lglut
PLIB_LIBS := -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt
MAGICK_LIBS := -lMagick++ -lMagickCore
BOOST_LIBS := -lboost_system -lboost_timer -lboost_chrono -lboost_thread
LIBS :=  $(OPENGL_LIBS) $(PLIB_LIBS) $(MAGICK_LIBS) $(BOOST_LIBS) -ljpeg  -lrt -fopenmp -Wc++0x-compat

# SHELL=/bin/bash

PROJDIRS := src/ src/ShapeFiles src/Observation src/Parallelization src/Physics src/Windows
# SRC_FILES := $(wildcard $(PROJDIRS)*.cpp)

SRC_FILES := $(wildcard src/*.cpp)
SRC_FILES += $(wildcard src/ShapeFiles/*.cpp)
SRC_FILES += $(wildcard src/Observation/*.cpp)
SRC_FILES += $(wildcard src/Parallelization/*.cpp)
SRC_FILES += $(wildcard src/Physics/*.cpp)
SRC_FILES += $(wildcard src/Windows/*.cpp)


# PROJDIRS := src/ src/ShapeFiles src/Observation src/Parallelization src/Physics src/Windows
# SRCFILES_NEW := $(shell find $(PROJDIRS) -type f -name "\*.cpp")

BUILD_FILES := $(subst src,Release/src,$(SRC_FILES))

OBJ_FILES := $(patsubst %.cpp,%.o,$(BUILD_FILES))
# OBJ_FILES := $(patsubst src/%.cpp,./Release/src/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst %.cpp,%.d,$(BUILD_FILES))


# All Target
all: ShapesLibrary
	echo Boobs

OBJS: 
	# $(foreach var,$(NUMBERS),./a.out $(var);)
	$(foreach var,$(PROJDIRS),echo $(var);)

# Tool invocations
# ShapesLibrary: ../Release/src/%.o $(OBJS) $(USER_OBJS)
ShapesLibrary: $(OBJ_FILES) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -Wall -g  -o"ShapesLibrary" $(OBJ_FILES) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '


	# @echo "replacement: "$(patsubst ./Release/src/%.o,src/%.cpp,$(@)) 
	# dependency:=$(@:./Release/src/%.o,src/%.cpp)
# $(OBJ_FILES): $(@:.o=.cpp) makefile
Release/src/%.o: ./src/%.cpp

	@echo "Cur object: $(@)"
	$(CC) $(CFLAGS) -std=c++11 -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -o "$@"   "$<"

# Other Targets
clean:
	-$(RM) $(OBJ_FILES)$(DEP_FILES) $(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ShapesLibrary
	-@echo ' '

clean-shapes:
	-$(RM) $(SHAPE_OBJS)$(SHAPE_CPP_DEPS) ShapesLibrary
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
