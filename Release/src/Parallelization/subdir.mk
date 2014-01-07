################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./src/Parallelization/Quadrant.cpp 


OBJS += \
./src/Parallelization/Quadrant.o 

CPP_DEPS += \
./src/Parallelization/Quadrant.d 



# Each subdirectory must supply rules for building sources it contributes
src/Parallelization/%.o: ../src/Parallelization/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Wall  -g -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


