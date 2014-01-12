################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
# ../src/ShapeFiles/%.cpp

SHAPE_OBJS += \
./src/ShapeFiles/MyShape.o \
./src/ShapeFiles/Moveable.o \
./src/ShapeFiles/MyEllipse.o \
./src/ShapeFiles/Box.o \
./src/ShapeFiles/Circle.o \
./src/ShapeFiles/RoundShape.o \
./src/ShapeFiles/ShapeList.o \
./src/ShapeFiles/StraightShape.o 

OBJS += $(SHAPE_OBJS)

SHAPE_CPP_DEPS += \
./src/ShapeFiles/%.d

# CPP_DEPS += $(SHAPE_CPP_DEPS)
CPP_DEPS += \
./src/ShapeFiles/MyShape.d \
./src/ShapeFiles/Moveable.d \
./src/ShapeFiles/MyEllipse.d \
./src/ShapeFiles/Box.d \
./src/ShapeFiles/Circle.d \
./src/ShapeFiles/RoundShape.d \
./src/ShapeFiles/ShapeList.d \
./src/ShapeFiles/StraightShape.d 

# Each subdirectory must supply rules for building sources it contributes
src/ShapeFiles/%.o: ../src/ShapeFiles/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Wall  -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


