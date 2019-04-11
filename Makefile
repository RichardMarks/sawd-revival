# Makefile
# Compiles the Game Executable

TARGET_EXEC ?= game
TARGET_DIR ?= ./dist
BUILD_DIR ?= ./build
SOURCE_DIRS ?= ./src
INCLUDE_DIRS := $(shell find $(SOURCE_DIRS) -type d)
RESOURCES_DIR ?= ./assets

LIBRARY_COMPILER_FLAGS ?= $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --cflags)
LIBRARY_LINKER_FLAGS ?= $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --libs)

COPY_RESOURCES ?= rsync -rvui --progress --exclude=.gitkeep
MKDIR_P ?= mkdir -p
COMPILER ?= clang++

# production
# PREPROC_DEFINES ?= -DNDEBUG
# dev
PREPROC_DEFINES ?= -DDEBUG

SOURCES := $(shell find $(SOURCE_DIRS) -name *.cpp)
OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
CPPFLAGS ?= $(INCLUDE_FLAGS) $(PREPROC_DEFINES) -MMD -MP -g -std=c++14
CFLAGS ?= $(LIBRARY_COMPILER_FLAGS) -O0
LDFLAGS ?= $(LIBRARY_LINKER_FLAGS) -Wl,-headerpad_max_install_names

.PHONY: clean
.PHONY: resources

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJECTS)
	$(MKDIR_P) $(TARGET_DIR)
	$(COMPILER) $(OBJECTS) -o $@ $(LDFLAGS)
	$(COPY_RESOURCES) $(RESOURCES_DIR)/ $(TARGET_DIR)/

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(COMPILER) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(TARGET_DIR)

resources:
	$(COPY_RESOURCES) $(RESOURCES_DIR)/ $(TARGET_DIR)/

-include $(DEPENDENCIES)
