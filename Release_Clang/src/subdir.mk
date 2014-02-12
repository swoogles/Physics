################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BillProperties.cpp \
../src/fileInteraction.cpp \
../src/inputFunctions.cpp \
../src/main.cpp \
../src/menus.cpp 

OBJS += \
./src/BillProperties.o \
./src/fileInteraction.o \
./src/inputFunctions.o \
./src/main.o \
./src/menus.o 

CPP_DEPS += \
./src/BillProperties.d \
./src/fileInteraction.d \
./src/inputFunctions.d \
./src/main.d \
./src/menus.d 

CPLUSPLUS_COMPILER=GCC

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: $(CPLUSPLUS_COMPILER) C++ Linker'
	clang++ -std=c++0x  -Wall  -g -fopenmp -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


