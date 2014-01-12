################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ShapeFiles/%.cpp

SHAPE_OBJS += \
./src/ShapeFiles/%.o

OBJS += $(SHAPE_OBJS)

SHAPE_CPP_DEPS += \
./src/ShapeFiles/%.d

CPP_DEPS += $(SHAPE_CPP_DEPS)

# Each subdirectory must supply rules for building sources it contributes
src/ShapeFiles/%.o: ../src/ShapeFiles/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Wall  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


