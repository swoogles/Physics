################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./src/Observation/Observer.cpp \
./src/Observation/Recorder.cpp 
# ../src/Physics/Interactions.cpp \
# ../src/Physics/Simulations.cpp \
# ../src/Physics/WorldSettings.cpp 


OBJS += \
./src/Observation/Observer.o \
./src/Observation/Recorder.o 
# ./src/Physics/Interactions.o \
# ./src/Physics/Simulations.o \
# ./src/Physics/WorldSettings.o 


CPP_DEPS += \
./src/Observation/Observer.d \
./src/Observation/Recorder.d 
# ./src/Physics/Interactions.d \
# ./src/Physics/Simulations.d \
# ./src/Physics/WorldSettings.d 



# Each subdirectory must supply rules for building sources it contributes
src/Observation/%.o: ../src/Observation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


