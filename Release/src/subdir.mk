################################################################################
# Automatically-generated file. Do not edit!
################################################################################
SHELL=/bin/bash

# MODULES := ls -d ./src/*/
# MODULES := ShapeFiles Observation

SRC_FILES := $(wildcard ../src/*.cpp)
SRC_FILES += $(wildcard ../src/ShapeFiles/*.cpp)
SRC_FILES += $(wildcard ../src/Observation/*.cpp)
SRC_FILES += $(wildcard ../src/Parallelization/*.cpp)
SRC_FILES += $(wildcard ../src/Physics/*.cpp)
SRC_FILES += $(wildcard ../src/Windows/*.cpp)

# PROJDIRS := ../src/ ../src/ShapeFiles ../src/Observation ../src/Parallelization ../src/Physics ../src/Windows
PROJDIRS := ../src/ ../src/ShapeFiles ../src/Observation ../src/Parallelization ../src/Physics ../src/Windows
SRCFILES_NEW := $(shell find $(PROJDIRS) -type f -name "\*.cpp")

ENDING = .cpp
OBJ_TEST_TMP := $(SRC_FILES:$(ENDING)=.o)
OBJ_TEST := $(subst ..,.,$(OBJ_TEST_TMP))
OBJS=$(OBJ_TEST)
# And this will replace all endings:

# OBJ = $(addsuffix .o,$(basename $(SOURCES)))



# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS = $(SRC_FILES)

CPP_DEPS += \
./src/BillProperties.d \
./src/fileInteraction.d \
./src/inputFunctions.d \
./src/main.d \
./src/menus.d 

# SRC_FILES := $(../../%.cpp)
# OBJ := $(SRC:.c=.o)
# test: $(OBJ)
# 	$(CC) -o $@ $(OBJ)
# 	include $(OBJ:.o=.d)
# 	%.d: %.c
# 	depend.sh $(CFLAGS) \
# 		$*.c > $@

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	echo $(PROJDIRS)
	echo $(SRCFILES_NEW)
	
	$(CC) -std=c++11 -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


