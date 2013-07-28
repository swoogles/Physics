################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ShapeFiles/Box.cpp \
../src/ShapeFiles/Circle.cpp \
../src/ShapeFiles/MyEllipse.cpp \
../src/ShapeFiles/MyShape.cpp \
../src/ShapeFiles/Moveable.cpp \
../src/ShapeFiles/RoundShape.cpp \
../src/ShapeFiles/StraightShape.cpp 

OBJS += \
./src/ShapeFiles/Box.o \
./src/ShapeFiles/Circle.o \
./src/ShapeFiles/MyEllipse.o \
./src/ShapeFiles/MyShape.o \
./src/ShapeFiles/Moveable.o \
./src/ShapeFiles/RoundShape.o \
./src/ShapeFiles/StraightShape.o 

CPP_DEPS += \
./src/ShapeFiles/Box.d \
./src/ShapeFiles/Circle.d \
./src/ShapeFiles/MyEllipse.d \
./src/ShapeFiles/MyShape.d \
./src/ShapeFiles/Moveable.d \
./src/ShapeFiles/RoundShape.d \
./src/ShapeFiles/StraightShape.d 


# Each subdirectory must supply rules for building sources it contributes
src/ShapeFiles/%.o: ../src/ShapeFiles/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


