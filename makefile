CC=g++ # I think this should be set in the environment, rather than in the file

TARGET_EXEC ?= PhysicsSandbox
TEST_TARGET_EXEC ?= Tests

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

TEST_SRC_DIRS ?= ./Test

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

TEST_SRCS := $(shell find $(TEST_SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s) $(SRCS)
TEST_OBJS := $(filter-out ./build/./src/main.cpp.o,  $(TEST_SRCS:%=$(BUILD_DIR)/%.o))
TEST_DEPS := $(TEST_OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Platform detection
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS configuration
    HOMEBREW_PREFIX := $(shell brew --prefix)
    PLIB_PREFIX := $(HOME)/.local

    # OpenGL and GLUT frameworks (native macOS)
    OPENGL_LIBS := -framework OpenGL -framework GLUT
    # Use local include path for GL compatibility headers, then homebrew for other GL headers
    OPENGL_INCLUDE := -I$(HOME)/.local/include -I$(HOMEBREW_PREFIX)/include

    # plib paths
    PLIB_INCLUDE := -I$(PLIB_PREFIX)/include
    PLIB_LIB_PATH := -L$(PLIB_PREFIX)/lib
    PLIB_LIBS := $(PLIB_LIB_PATH) -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt

    # Boost from Homebrew (thread requires -mt suffix on this version)
    BOOST_LIBS := -L$(HOMEBREW_PREFIX)/lib -lboost_system -lboost_timer -lboost_chrono -lboost_thread-mt
    BOOST_INCLUDE := -I$(HOMEBREW_PREFIX)/include

    # OpenMP with Apple Clang
    OPENMP_FLAGS := -Xpreprocessor -fopenmp -I$(HOMEBREW_PREFIX)/opt/libomp/include
    OPENMP_LIBS := -L$(HOMEBREW_PREFIX)/opt/libomp/lib -lomp

    # jpeg from Homebrew
    JPEG_LIB := -L$(HOMEBREW_PREFIX)/lib -ljpeg

    # No -lrt on macOS
    RT_LIB :=

    # Silence OpenGL deprecation warnings on macOS
    PLATFORM_FLAGS := -DGL_SILENCE_DEPRECATION
else
    # Linux configuration
    OPENGL_LIBS := -lGL -lGLU -lglut
    OPENGL_INCLUDE :=
    PLIB_INCLUDE :=
    PLIB_LIB_PATH :=
    PLIB_LIBS := -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt
    BOOST_LIBS := -lboost_system -lboost_timer -lboost_chrono -lboost_thread
    BOOST_INCLUDE :=
    OPENMP_FLAGS := -fopenmp
    OPENMP_LIBS :=
    JPEG_LIB := -ljpeg
    RT_LIB := -lrt
    PLATFORM_FLAGS :=
endif

# IMAGE_LIBS := -lMagick++ -lMagickCore
# IM_CXXFLAGS=$(Magick++-config --cxxflags)
# IM_LDFLAGS=$(Magick++config --ldflags)
LIBS := $(OPENGL_LIBS) $(PLIB_LIBS) $(BOOST_LIBS) $(JPEG_LIB) $(RT_LIB) $(OPENMP_LIBS)

COMPILE_ONLY_OPTION := -c
STANDARD_OPTIONS := -std=c++17
WARNINGS_OPTIONS := -Wall
DEBUG_OPTIONS := -g -O3

COMPILE_OPTIONS = $(DEBUG_OPTIONS) $(WARNINGS_OPTIONS) $(STANDARD_OPTIONS) $(PLATFORM_FLAGS) $(OPENMP_FLAGS)

CPPFLAGS ?= $(COMPILE_OPTIONS) $(COMPILE_ONLY_OPTION) $(INC_FLAGS) $(PLIB_INCLUDE) $(BOOST_INCLUDE) $(OPENGL_INCLUDE) -MMD -MP

main: $(BUILD_DIR)/$(TARGET_EXEC)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

test: $(BUILD_DIR)/$(TEST_TARGET_EXEC)
$(BUILD_DIR)/$(TEST_TARGET_EXEC): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $@ $(LDFLAGS) $(LIBS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

all: main test

-include $(DEPS)

MKDIR_P ?= mkdir -p
