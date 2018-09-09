CC=g++ # I think this should be set in the environment, rather than in the file

TARGET_EXEC ?= PhysicsSandbox

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

OPENGL_LIBS := -lGL -lGLU -lglut
PLIB_LIBS := -lplibpuaux -lplibpu -lplibsg -lplibul -lplibfnt
BOOST_LIBS := -lboost_system -lboost_timer -lboost_chrono -lboost_thread
LIBS :=  $(OPENGL_LIBS) $(PLIB_LIBS) $(BOOST_LIBS) -ljpeg  -lrt -fopenmp -Wc++0x-compat

COMPILE_ONLY_OPTION := -c
STANDARD_OPTIONS := -std=c++11 
WARNINGS_OPTIONS := -Wall 
DEBUG_OPTIONS := -g -O3 

COMPILE_OPTIONS = $(DEBUG_OPTIONS) $(WARNINGS_OPTIONS) $(STANDARD_OPTIONS)


CPPFLAGS ?= $(COMPILE_OPTIONS) $(COMPILE_ONLY_OPTION) $(INC_FLAGS) -MMD -MP $(LIBS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
