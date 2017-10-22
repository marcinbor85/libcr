#
# The MIT License (MIT)
# 
# Copyright (c) 2017 Marcin Borowicz <marcinbor85@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

######################################################################################

GCC_BASE = /opt/gcc-arm-none-eabi-5_2-2015q4/
GCC_BIN  = $(GCC_BASE)bin/
GCC_LIB  = $(GCC_BASE)arm-none-eabi/lib/
GCC_INC  = $(GCC_BASE)arm-none-eabi/include/
AS       = $(GCC_BIN)arm-none-eabi-as
CC       = $(GCC_BIN)arm-none-eabi-gcc
CPP      = $(GCC_BIN)arm-none-eabi-g++
LD       = $(GCC_BIN)arm-none-eabi-gcc
OBJCOPY  = $(GCC_BIN)arm-none-eabi-objcopy
SIZE     = $(GCC_BIN)arm-none-eabi-size

STFLASH = st-flash

######################################################################################

CORTEX_M0PLUS_CC_FLAGS  = -mthumb -mcpu=cortex-m0plus
CORTEX_M0PLUS_LIB_PATH  = $(GCC_LIB)armv6-m
CORTEX_M0_CC_FLAGS      = -mthumb -mcpu=cortex-m0
CORTEX_M0_LIB_PATH      = $(GCC_LIB)armv6-m
CORTEX_M1_CC_FLAGS      = -mthumb -mcpu=cortex-m1
CORTEX_M1_LIB_PATH      = $(GCC_LIB)armv6-m
CORTEX_M3_CC_FLAGS      = -mthumb -mcpu=cortex-m3
CORTEX_M3_LIB_PATH      = $(GCC_LIB)armv7-m
CORTEX_M4_NOFP_CC_FLAGS = -mthumb -mcpu=cortex-m4
CORTEX_M4_NOFP_LIB_PATH = $(GCC_LIB)armv7e-m
CORTEX_M4_SWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CORTEX_M4_SWFP_LIB_PATH = $(GCC_LIB)armv7e-m/softfp
CORTEX_M4_HWFP_CC_FLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
CORTEX_M4_HWFP_LIB_PATH = $(GCC_LIB)armv7e-m/fpu
CORTEX_R4_NOFP_CC_FLAGS = -mthumb -march=armv7-r
CORTEX_R4_NOFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb
CORTEX_R4_SWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=softfp -mfpu=vfpv3-d16
CORTEX_R4_SWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/softfp
CORTEX_R4_HWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=hard -mfpu=vfpv3-d16
CORTEX_R4_HWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/fpu
CORTEX_R5_NOFP_CC_FLAGS = -mthumb -march=armv7-r
CORTEX_R5_NOFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb
CORTEX_R5_SWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=softfp -mfpu=vfpv3-d16
CORTEX_R5_SWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/softfp
CORTEX_R5_HWFP_CC_FLAGS = -mthumb -march=armv7-r -mfloat-abi=softfp -mfloat-abi=hard -mfpu=vfpv3-d16
CORTEX_R5_HWFP_LIB_PATH = $(GCC_LIB)armv7-r/thumb/fpu

######################################################################################

PROJECT           = example1_nucleo_stm32f072
VERSION           = v0.1.0
DATE		  = $(shell date +%Y%m%d-%H%M%S)
MCU_CC_FLAGS      = $(CORTEX_M0_CC_FLAGS)
MCU_LIB_PATH      = $(CORTEX_M0_LIB_PATH)
OPTIMIZE_FOR      = 
DEBUG_LEVEL       = 
OPTIM_LEVEL       = 
LINKER_SCRIPT     = stm32f072_flash.ld
PROJECT_SYMBOLS   = -DSTM32F072 -DUSE_STDPERIPH_DRIVER

BIN_DIR = ../../$(PROJECT)
OUT_DIR = $(BIN_DIR)/out

SRC_DIR = ..
SRC_DIRS = $(SRC_DIR)/cr $(SRC_DIR)/stm32 $(SRC_DIR)/stm32/startup $(SRC_DIR)/stm32/StdPeriph_Driver/src $(SRC_DIR)/stm32/STM32F0xx-Nucleo
INC_DIRS = $(SRC_DIR) $(SRC_DIR)/stm32 $(SRC_DIR)/stm32/CMSIS/core $(SRC_DIR)/stm32/CMSIS/device $(SRC_DIR)/stm32/StdPeriph_Driver/inc $(SRC_DIR)/stm32/STM32F0xx-Nucleo
LIB_DIRS = 
LIBS = 

VPATH = $(SRC_DIRS)

PROJECT_INC_PATHS = $(patsubst %, -I%, $(INC_DIRS))
PROJECT_LIB_PATHS = $(patsubst %, -L%, $(LIB_DIRS))
PROJECT_LIBRARIES = $(patsubst %, -l%, $(LIBS))

C_SRCS   = $(wildcard $(patsubst %, %/*.c, $(SRC_DIRS)))
AS_SRCS  = $(wildcard $(patsubst %, %/*.S, $(SRC_DIRS)))

C_OBJS   = $(addprefix $(OUT_DIR)/, $(notdir $(C_SRCS:.c=.o)))
AS_OBJS  = $(addprefix $(OUT_DIR)/, $(notdir $(AS_SRCS:.S=.o)))

PROJECT_OBJECTS = $(AS_OBJS) $(C_OBJS)

######################################################################################

SYS_OBJECTS = 
SYS_INC_PATHS = -I$(GCC_INC) 
SYS_LIB_PATHS = -L$(MCU_LIB_PATH)
ifeq (OPTIMIZE_FOR, SIZE)
SYS_LIBRARIES = -lstdc++_s -lsupc++_s -lm -lc_s -lg_s -lnosys
SYS_LD_FLAGS  = --specs=nano.specs -u _printf_float -u _scanf_float
else 
SYS_LIBRARIES = -lstdc++ -lsupc++ -lm -lc -lg -lnosys
SYS_LD_FLAGS  = 
endif

############################################################################### 

ifdef DEBUG_LEVEL
CC_DEBUG_FLAGS = -g$(DEBUG_LEVEL)
CC_SYMBOLS = -DDEBUG $(PROJECT_SYMBOLS)
else
CC_DEBUG_FLAGS =
CC_SYMBOLS = -DNODEBUG $(PROJECT_SYMBOLS)
endif 

ifdef OPTIM_LEVEL
CC_OPTIM_FLAGS = -O$(OPTIM_LEVEL)
else 
CC_OPTIM_FLAGS = 
endif

BUILD_TARGET = $(BIN_DIR)/$(PROJECT)
ELF = $(BUILD_TARGET).elf
BIN = $(BUILD_TARGET).bin
HEX = $(BUILD_TARGET).hex
MAP = $(BUILD_TARGET).map

INCLUDE_PATHS = $(PROJECT_INC_PATHS) $(SYS_INC_PATHS)
LIBRARY_PATHS = $(PROJECT_LIB_PATHS) $(SYS_LIB_PATHS)
CC_FLAGS = $(MCU_CC_FLAGS) -c $(CC_OPTIM_FLAGS) $(CC_DEBUG_FLAGS) -fno-common -fmessage-length=0 -Wall -fno-exceptions -ffunction-sections -fdata-sections 
LD_FLAGS = $(MCU_CC_FLAGS) -Wl,--gc-sections $(SYS_LD_FLAGS) -Wl,-Map=$(MAP) 
LD_SYS_LIBS = $(SYS_LIBRARIES)

############################################################################### 

all: make_output_dir $(HEX) $(BIN) print_size
	
make_output_dir:
	$(shell mkdir $(BIN_DIR) 2>/dev/null)
	$(shell mkdir $(OUT_DIR) 2>/dev/null)
	
print_size :
	@echo 'Size of modules:'
	$(SIZE) -B -t --common $(PROJECT_OBJECTS)
	@echo ' '
	@echo 'Size of target .elf file:'
	$(SIZE) -B $(BUILD_TARGET).elf
	@echo ' '

clean:
	rm -f $(BIN) $(HEX) $(ELF) $(MAP) $(PROJECT_OBJECTS)

$(OUT_DIR)/%.o: %.S
	$(AS) $(MCU_CC_FLAGS) -o $@ $<

$(OUT_DIR)/%.o: %.c
	$(CC)  $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99   $(INCLUDE_PATHS) -o $@ $<

$(ELF): $(PROJECT_OBJECTS) $(SYS_OBJECTS)
	$(LD) $(LD_FLAGS) -T$(LINKER_SCRIPT) $(LIBRARY_PATHS) -o $@ $^ $(PROJECT_LIBRARIES) $(SYS_LIBRARIES)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex --set-start 0 $< $@
	
flash:
	$(STFLASH) --format ihex write $(HEX)
