RM := rm -rf

CC=g++

# All of the sources participating in the build are defined here
# 32 Bit/Laptop version
OPENGL_LIBS := -lGL -lGLU -lglut
PLIB_LIBS := -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt
MAGICK_LIBS := -lMagick++ -lMagickCore
BOOST_LIBS := -lboost_system -lboost_timer -lboost_chrono -lboost_thread
LIBS :=  $(OPENGL_LIBS) $(PLIB_LIBS) $(MAGICK_LIBS) $(BOOST_LIBS) -ljpeg  -lrt -fopenmp -Wc++0x-compat

CPP_EXT = .cpp

SRC_FILES := $(wildcard ../src/*$(CPP_EXT))
SRC_FILES += $(wildcard ../src/ShapeFiles/*$(CPP_EXT))
SRC_FILES += $(wildcard ../src/Observation/*$(CPP_EXT))
SRC_FILES += $(wildcard ../src/Parallelization/*$(CPP_EXT))
SRC_FILES += $(wildcard ../src/Physics/*$(CPP_EXT))
SRC_FILES += $(wildcard ../src/Windows/*$(CPP_EXT))

BUILD_FILES := $(subst ..,.,$(SRC_FILES))

OBJS:= $(BUILD_FILES:$(CPP_EXT)=.o)
CPP_DEPS:= $(BUILD_FILES:$(CPP_EXT)=.d)

# All Target
all: ShapesLibrary

# Tool invocations
ShapesLibrary: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -Wall -g  -o"ShapesLibrary" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) ShapesLibrary
	-@echo ' '

clean-shapes:
	-$(RM) $(SHAPE_OBJS)$(SHAPE_CPP_DEPS) ShapesLibrary
	-@echo ' '

# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	$(CC) -std=c++11 -g -Imgl -Iplibsg -Iplibul -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'bill bilding'
	@echo 'Finished building: $<'
	@echo ' '

.PHONY: all clean dependents
.SECONDARY:
