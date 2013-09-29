################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Windows/control_center.cpp \
../src/Windows/main_display.cpp 

OBJS += \
./src/Windows/control_center.o \
./src/Windows/main_display.o 

CPP_DEPS += \
./src/Windows/control_center.d \
./src/Windows/main_display.d 


# Each subdirectory must supply rules for building sources it contributes
src/Windows/%.o: ../src/Windows/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


