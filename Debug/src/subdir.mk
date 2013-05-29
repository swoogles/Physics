################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Box.cpp \
../src/Circle.cpp \
../src/MyEllipse.cpp \
../src/MyShape.cpp \
../src/RoundShape.cpp \
../src/StraightShape.cpp \
../src/globals.cpp \
../src/inputFunctions.cpp \
../src/main.cpp \
../src/menus.cpp 

OBJS += \
./src/Box.o \
./src/Circle.o \
./src/MyEllipse.o \
./src/MyShape.o \
./src/RoundShape.o \
./src/StraightShape.o \
./src/globals.o \
./src/inputFunctions.o \
./src/main.o \
./src/menus.o 

CPP_DEPS += \
./src/Box.d \
./src/Circle.d \
./src/MyEllipse.d \
./src/MyShape.d \
./src/RoundShape.d \
./src/StraightShape.d \
./src/globals.d \
./src/inputFunctions.d \
./src/main.d \
./src/menus.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


