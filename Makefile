#####################################################################################################
#																									#
#						Classic Chen & Co Generic Makefile by BlivionIaG							#
#																									#
#####################################################################################################

# Tools
###_TOOL_BLOCK_###
R_WILDCARD=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call R_WILDCARD,$d/,$2))
FILTER_OUT=$(foreach v,$(2),$(if $(findstring $(1),$(v)),,$(v)))
empty:=
###_END_BLOCK_###

# SSH/SCP credentials
REMOTE_USER = root
REMOTE_HOST = 127.0.0.1
REMOTE_PATH = ~/

# Build config and output
BINARY_NAME		=	OhMyBZH-Generator
OUTPUT_FOLDER	=	.
SRC_DIR			=	./src
PROJECT_TYPE	=	C++
STD_NORM		=	c++17
CFLAGS 			= 	-g -Wall -Wextra -Os -Wno-implicit-fallthrough
LDFLAGS 		= 	-licuuc -lpqxx
CFLAGS 			+= 	$(shell mapnik-config --includes --defines --cxxflags --dep-includes)
LDFLAGS 		+= 	$(shell mapnik-config --libs --dep-libs --ldflags)

# Info output
$(info Make started, binary name : $(BINARY_NAME))
$(info Output folder : $(OUTPUT_FOLDER))
$(info )

# Project Type
ifneq (,$(filter c C,$(PROJECT_TYPE)))				# C Project Type	
	SRC_EXT = c
	INC_EXT = h
	COMPILER = gcc
	STD_NORM = gnu11
else ifneq (,$(filter c++ C++,$(PROJECT_TYPE)))		# C++ Project Type
	SRC_EXT = cpp
	INC_EXT = hpp
	COMPILER = g++
	STD_NORM = c++17
else												# Default Project Type
	PROJECT_TYPE = C++
	SRC_EXT = cpp
	INC_EXT = hpp
	COMPILER = g++
	STD_NORM = c++17
endif
$(info $(PROJECT_TYPE) Project detected, source files are *.$(SRC_EXT), headers files are *.$(INC_EXT), using $(COMPILER) with --std=$(STD_NORM))
$(info )


ifneq (,$(filter rpi1 RPI1,$(TARGET)))
	COMPILER_PREFIX = arm-linux-gnueabihf-
	TARGET_CFLAGS = -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -mtune=arm1176jzf-s -static-libstdc++
	EXTENSION = -rpi1
else ifneq (,$(filter rpi2 RPI2,$(TARGET)))
	COMPILER_PREFIX = arm-linux-gnueabihf-
	TARGET_CFLAGS = -mcpu=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4 -mtune=cortex-a7 -static-libstdc++
	EXTENSION = -rpi2
else ifneq (,$(filter rpi3 RPI3,$(TARGET)))
	COMPILER_PREFIX = arm-linux-gnueabihf-
	TARGET_CFLAGS = -mcpu=cortex-a53 -mfloat-abi=hard -mfpu=neon-fp-armv8 -mneon-for-64bits -mtune=cortex-a53 -static-libstdc++
	EXTENSION = -rpi3
else ifneq (,$(filter beaglebb BEAGLEBB,$(TARGET)))
	COMPILER_PREFIX = arm-linux-gnueabihf-
	TARGET_CFLAGS = -mcpu=cortex-a8 -mfloat-abi=hard -mfpu=neon -mtune=cortex-a8 -static-libstdc++
	EXTENSION = -beaglebb
endif
ifneq ($(TARGET),)
	$(info Target $(TARGET) specified, using optimized CFLAGS : $(TARGET_CFLAGS) and $(EXTENSION) extension)
	$(info )
endif


# Source files detection
ALLSRC := $(call R_WILDCARD,,*.$(SRC_EXT))
FILTERED_SRC = $(call FILTER_OUT,MergeStructFiles,$(ALLSRC))
SRCDIR = $(sort $(dir $(FILTERED_SRC)))
$(info Detected source files : $(strip $(ALLSRC)))
$(info After filter : $(strip $(FILTERED_SRC)))
$(info Detected source files directories : $(strip $(SRCDIR)))
$(info )

# Header files detection
ALLINC := $(call R_WILDCARD,,*.$(INC_EXT))
FILTERED_INC = $(call FILTER_OUT,MergeStructFiles,$(ALLINC))
INCDIR = $(sort $(dir $(FILTERED_INC)))
$(info Detected header files : $(strip $(ALLINC)))
$(info After filter : $(strip $(FILTERED_INC)))
$(info Detected header files directories : $(strip $(INCDIR)))
$(info )

# Lib files detection
ALLLIB := $(call R_WILDCARD,,*.a) $(call R_WILDCARD,,*.lib)
FILTERED_LIB = $(ALLLIB)					#$(call FILTER_OUT,THINGS_TO_FILTER,$(ALLLIB))
LIBDIR = $(sort $(dir $(FILTERED_LIB)))
$(info Detected library files : $(strip $(ALLLIB)))
$(info After filter : $(strip $(FILTERED_LIB)))
$(info Detected library files directories : $(strip $(LIBDIR)))
$(info )


# Includes / Libs / Flags
INCLUDES = $(addprefix -I,$(subst $(addsuffix /,$(CURDIR)),,$(realpath $(INCDIR)))) 	# include folders
CFLAGS += -std=$(STD_NORM) $(OPT_CFLAGS) $(TARGET_CFLAGS)								# Compiler flags
LDFLAGS += 																				# Libs for all systems
LDFLAGS += $(addprefix -L,$(subst $(addsuffix /,$(CURDIR)),,$(realpath $(LIBDIR))))		# Static libs inclusion
NOM = $(basename $(notdir $(FILTERED_SRC)))
OBJ = $(addprefix $(OUTPUT_FOLDER)/,$(addsuffix .o, $(NOM)))
$(info Detected includes : $(strip $(INCLUDES)))
$(info )

# Compiler choice
ifeq ($(CC),cc)									# If CC set to "cc"				
	CC = $(COMPILER_PREFIX)$(COMPILER)			# Forced to default compiler
else ifneq ($(CXX),)
	CC = $(COMPILER_PREFIX)$(CXX)
else ifeq ($(CC),)								# If no c or c++ compiler set
	CC = $(COMPILER_PREFIX)$(COMPILER)			# Forced to default compiler
endif
$(info Compiler set to $(CC))
$(info )

# OS Detection
ifeq ($(OS),Windows_NT)     					# Windows OS
    detected_OS := Windows
else
    detected_OS := $(shell uname) 				# Unix Based OS
endif
$(info OS detected as $(detected_OS))

# Apply specific parameters for Windows
ifeq ($(detected_OS),Windows)
	DELETE_CMD = del /F
	ifeq ($(TARGET),)
		EXTENSION = .exe
	endif
else
	DELETE_CMD = rm -f
endif
$(info Delete command set as "$(DELETE_CMD)" and binary extension as "$(EXTENSION)")
# DELETE CORRECTION FOR WINDOWS
ifeq ($(detected_OS),Windows)
	DELETE_OBJ = $(subst /,"\", $(OBJ))
	DELETE_EXE = $(subst /,"\", $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION))
else 
	DELETE_OBJ = $(OBJ)
	DELETE_EXE = $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION)
endif
$(info Object file delete target is now : $(DELETE_OBJ))
$(info )

$(info Compiler Flags : $(CFLAGS))
$(info )
$(info Linker Flags : $(LDFLAGS))
$(info )
$(info Generated files name : $(NOM))
$(info Object files target : $(OBJ))
$(info )

all: $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION)									# Build all executables
	@echo "\e[94;1mautomake: All done ($(shell date +%T))\e[0m"

rebuild: remake																		# Clean and build all executables
	$(info "remake" alias)

remake: clean $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION)							# Same as rebuild
	$(info Cleaning and Building everything ...)

$(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION): $(OBJ)
	$(strip $(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION))
	$(info )
	$(info Creating $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION) ...)
	$(info )

# Build all the .o files
$(OUTPUT_FOLDER)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(strip $(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@)
	$(info )
	$(info Building $@ with $< dependencies using $(strip $(CC)) with $(strip $(CFLAGS)) Flags)
	$(info )

# Remove all files generated by compilation (executables included)
clean:
	$(DELETE_CMD) $(DELETE_OBJ) $(DELETE_EXE) *.gch

# Remove all files generated by compilation (except executables)
clear:
	$(DELETE_CMD) $(DELETE_OBJ) *.gch

run: $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION)
	$(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION)

upload: remake
	scp $(OUTPUT_FOLDER)/$(BINARY_NAME)$(EXTENSION) $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_PATH)

remote-run: upload
	ssh $(REMOTE_USER)@$(REMOTE_HOST) '$(REMOTE_PATH)$(BINARY_NAME)$(EXTENSION)'

kill:
	ssh $(REMOTE_USER)@$(REMOTE_HOST) 'killall -s 2 $(BINARY_NAME)$(EXTENSION)'

maps:
	sh data/getMaps.sh