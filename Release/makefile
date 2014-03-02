RM := rm -rf

CC=g++

# All of the sources participating in the build are defined here
# -include src/subdir.mk

# 32 Bit/Laptop version
LIBS := -lGL -lGLU -lglut -ljpeg -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt -lMagick++ -lMagickCore -lboost_system -lboost_timer -lboost_chrono -lboost_thread -lrt -fopenmp -Wc++0x-compat

SRC_FILES := $(wildcard ../src/*.cpp)
SRC_FILES += $(wildcard ../src/ShapeFiles/*.cpp)
SRC_FILES += $(wildcard ../src/Observation/*.cpp)
SRC_FILES += $(wildcard ../src/Parallelization/*.cpp)
SRC_FILES += $(wildcard ../src/Physics/*.cpp)
SRC_FILES += $(wildcard ../src/Windows/*.cpp)

ENDING = .cpp
OBJ_TEST_TMP := $(SRC_FILES:$(ENDING)=.o)
OBJ_TEST := $(subst ..,.,$(OBJ_TEST_TMP))
OBJS=$(OBJ_TEST)
# And this will replace all endings:

# OBJ = $(addsuffix .o,$(basename $(SOURCES)))
CPP_DEPS_TEST_TMP := $(SRC_FILES:$(ENDING)=.d)
CPP_DEPS_TEST := $(subst ..,.,$(CPP_DEPS_TEST_TMP))
CPP_DEPS=$(CPP_DEPS_TEST)

# SRC_FILES := $(../../%.cpp)
# OBJ := $(SRC:.c=.o)
# test: $(OBJ)
# 	$(CC) -o $@ $(OBJ)
# 	include $(OBJ:.o=.d)
# 	%.d: %.c
# 	depend.sh $(CFLAGS) \
# 		$*.c > $@



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
