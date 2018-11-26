################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ShapeFiles/Box.cpp \
../src/ShapeFiles/Particle.cpp \
../src/ShapeFiles/MyShape.cpp \
../src/ShapeFiles/ShapeList.cpp 

OBJS += \
./src/ShapeFiles/Box.o \
./src/ShapeFiles/Particle.o \
./src/ShapeFiles/MyShape.o \
./src/ShapeFiles/ShapeList.o 

CPP_DEPS += \
./src/ShapeFiles/Box.d \
./src/ShapeFiles/Particle.d \
./src/ShapeFiles/MyShape.d \
./src/ShapeFiles/ShapeList.d 


# Each subdirectory must supply rules for building sources it contributes
src/ShapeFiles/%.o: ../src/ShapeFiles/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	clang++ -std=c++0x  -Wall  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


