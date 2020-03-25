# target
TARGET = app
# Optimization
OPT = -O2

BUILD_DIR = Bin

include MakeFile_Path
GCC_PATH = $(ROOT_DIR)/Gnu/5.4_2016q3/bin
PREFIX = arm-none-eabi-
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
AR = $(GCC_PATH)/$(PREFIX)ar
SZ = $(GCC_PATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

C_INCLUDE = \
-I$(LIB_DIR)/Lib/Include \
-I$(LIB_DIR)/Lib/Include/CMSIS \
-I$(LIB_DIR)/Lib/Include/FATFS \
-I$(LIB_DIR)/Lib/Include/Helix \
-I$(LIB_DIR)/Lib/Include/LWIP_Include \
-I$(LIB_DIR)/Lib/Include/LWIP_Include/ipv4 \
-I$(LIB_DIR)/Lib/Include/LWIP_Include/netif \
-I$(LIB_DIR)/Lib/Include/STM32_USB_Library \
-I$(LIB_DIR)/Lib/Include/STM32F4xx_HAL_Driver \

LIBS = -lc -lm -lnosys \
$(LIB_DIR)/Lib/libarm_cortexM4lf_math.a \
$(LIB_DIR)/Lib/libmem.a \
$(LIB_DIR)/Lib/libhelix.a \
$(LIB_DIR)/Lib/libstm32hal.a \
$(LIB_DIR)/Lib/librtos.a \
$(LIB_DIR)/Lib/libfatfs.a \
$(LIB_DIR)/Lib/liblwip.a \
$(LIB_DIR)/Lib/liblwiparch.a \
$(LIB_DIR)/Lib/libstm32_usb.a \
# $(LIB_DIR)/Lib/libpng.a \

C_INCLUDE += \
-IConfig\
-IConfig/AUDIO/inc \
-IConfig/BSP/inc \
-IConfig/ETH/inc \
-IConfig/USB/inc \
-IConfig/HAL/inc \
-IConfig/BLE/inc \
-IConfig/M2/inc \
-IConfig/WIFI/inc \
-IProtocol/inc \
-IUser \

C_SOURCES = \
$(wildcard Config/AUDIO/src/*.c) \
$(wildcard Config/BSP/src/*.c) \
$(wildcard Config/ETH/src/*.c) \
$(wildcard Config/USB/src/*.c) \
$(wildcard Config/HAL/src/*.c) \
$(wildcard Config/BLE/src/*.c) \
$(wildcard Config/M2/src/*.c) \
$(wildcard Config/WIFI/src/*.c) \
$(wildcard Protocol/src/*.c) \
$(wildcard User/*.c)

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F429xx \
-DUSE_USB_OTG_FS \
-DARM_MATH_CM4 \
-DARM_MATH_MATRIX_CHECK \
-DARM_MATH_ROUNDING \
-D__FPU_PRESENT=1

# compile gcc flags
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDE) $(OPT) -Wall  -fdata-sections -ffunction-sections

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -fdata-sections 

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# link script
LDSCRIPT = STM32F429IGTx_FLASH.ld 

#-LDSP_LIB
LDFLAGS = $(MCU) -specs=nosys.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -lm
.PHONY:cleanapp all
# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@
	$(info compile file $@)
	
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile  
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@ 
	@$(SZ) $@
	
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
	
$(BUILD_DIR):
	mkdir $@
	
cleanod:
	-del $(BUILD_DIR)\*.o $(BUILD_DIR)\*.d $(BUILD_DIR)\*.lst

cleanapp:
	-del $(BUILD_DIR)\$(TARGET).bin $(BUILD_DIR)\$(TARGET).elf $(BUILD_DIR)\$(TARGET).hex $(BUILD_DIR)\$(TARGET).map