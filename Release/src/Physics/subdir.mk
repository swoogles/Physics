################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Physics/Interactions.cpp \
../src/Physics/Simulations.cpp \
../src/Physics/WorldSettings.cpp 

OBJS += \
./src/Physics/Interactions.o \
./src/Physics/Simulations.o \
./src/Physics/WorldSettings.o 

CPP_DEPS += \
./src/Physics/Interactions.d \
./src/Physics/Simulations.d \
./src/Physics/WorldSettings.d 


# Each subdirectory must supply rules for building sources it contributes
src/Physics/%.o: ../src/Physics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


