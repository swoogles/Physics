################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./src/Observation/Timer.cpp \
./src/Observation/Observer.cpp

OBJS += \
./src/Observation/Timer.o \
./src/Observation/Observer.o

CPP_DEPS += \
./src/Observation/Timer.d \
./src/Observation/Observer.d

# Each subdirectory must supply rules for building sources it contributes
src/Observation/%.o: ../src/Observation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	clang++ -std=c++0x  -Wall  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


