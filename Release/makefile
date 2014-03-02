################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

CC=g++

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk

# 32 Bit/Laptop version
LIBS := -lGL -lGLU -lglut -ljpeg -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt -lMagick++ -lMagickCore -lboost_system -lboost_timer -lboost_chrono -lboost_thread -lrt -fopenmp -Wc++0x-compat

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ShapesLibrary


# Tool invocations
ShapesLibrary: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -Wall -g  -o"ShapesLibrary" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ShapesLibrary
	-@echo ' '

clean-shapes:
	-$(RM) $(SHAPE_OBJS)$(SHAPE_CPP_DEPS) ShapesLibrary
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
