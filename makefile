# My best attempt at a good makefile so far
# There are still plenty of stupid things in here, but it's much
# better than it used to be.

RM := rm -rf

CC=g++

OPENGL_LIBS := -lGL -lGLU -lglut
PLIB_LIBS := -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt
BOOST_LIBS := -lboost_system -lboost_timer -lboost_chrono -lboost_thread
LIBS :=  $(OPENGL_LIBS) $(PLIB_LIBS) $(BOOST_LIBS) -ljpeg  -lrt -fopenmp -Wc++0x-compat

DEPENDENCY_OPTIONS := -MMD -MP -MF
DEPENDENCY_CONVERSION := 
DEPENDENCY_SECTION := $(DEPENDENCY_OPTIONS) $(DEPENDENCY_CONVERSION) 

COMPILE_ONLY_OPTION := -c
STANDARD_OPTIONS := -std=c++11 
WARNINGS_OPTIONS := -Wall 
DEBUG_OPTIONS := -g -O3 

# Might not be necessary
LINK_LIBS := -Imgl -Iplibsg -Iplibul 

COMPILE_OPTIONS = $(DEBUG_OPTIONS) $(WARNINGS_OPTIONS) $(STANDARD_OPTIONS)

PROJDIRS := src/ src/ShapeFiles src/Observation src/Parallelization src/Physics src/Windows
# SRC_FILES := $(wildcard $(PROJDIRS)*.cpp)

SRC_FILES := $(wildcard src/*.cpp)
SRC_FILES += $(wildcard src/*/?*.cpp)

BUILD_FILES := $(subst src,Release/src,$(SRC_FILES))
OBJ_FILES := $(patsubst %.cpp,%.o,$(BUILD_FILES))
DEP_FILES := $(patsubst %.cpp,%.d,$(BUILD_FILES))

MAIN_OBJECT := Release/src/main.o
OBJS_WITHOUT_MAIN := $(filter-out $(MAIN_OBJECT),$(OBJ_FILES))

#Test variables
TEST_SOURCE := "./Release/unitTests.cpp"
TEST_TMP := $(subst ./Release/,./Release/src/,$(TEST_SOURCE) )
TEST_DEPENDS := $(subst .cpp,.d,$(TEST_TMP) )
TEST_OBJECTS := $(subst .cpp,.o,$(TEST_TMP) )

# All Target
all: ShapesLibrary

OBJS: 
	$(foreach var,$(PROJDIRS),echo $(var);)

ShapesLibrary: $(OBJ_FILES) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -Wall -g  -o"ShapesLibrary" $(OBJ_FILES) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Tests: $(OBJ_FILES) 
	@echo 'Building target: $@'
	$(CC) $(COMPILE_OPTIONS) $(COMPILE_ONLY_OPTION) $(DEPENDENCY_OPTIONS) $(TEST_DEPENDS) -MT $(TEST_DEPENDS) -o $(TEST_OBJECTS) $(TEST_SOURCE)
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -Wall -g  -o"UnitTests" ./Release/src/unitTests.o $(OBJS_WITHOUT_MAIN)  $(LIBS)
	@echo 'Finished building target: $@'


# $(OBJ_FILES): $(@:.o=.cpp) makefile
Release/src/%.o: ./src/%.cpp
	@echo "Cur object: $(@)"
	$(CC) $(CFLAGS) $(COMPILE_OPTIONS) $(COMPILE_ONLY_OPTION) $(DEPENDENCY_OPTIONS) "$(@:%.o=%.d)" -MT "$(@:%.o=%.d)"  -o "$@"   "$<"

# Other Targets
clean:
	-$(RM) $(OBJ_FILES) $(DEP_FILES) ShapesLibrary
	-@echo ' '

clean-shapes:
	-$(RM) $(SHAPE_OBJS)$(SHAPE_CPP_DEPS) ShapesLibrary
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
