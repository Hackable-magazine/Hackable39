TARGET = prem
BUILD_DIR = build
LDSCRIPT = STM32L432KC.ld

ASM_SOURCES = \
premier.s

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
LD = $(PREFIX)ld
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
BIN = $(CP) -O binary -S
GDB = $(PREFIX)gdb-py

CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# compile gcc flags
CFLAGS = $(MCU) -Wall -fdata-sections -ffunction-sections

CFLAGS += -g -gdwarf-2 -O0

# Generate dependency information
#CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDFLAGS = $(MCU) -nostdlib -specs=nano.specs -T$(LDSCRIPT) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.s $(BUILD_DIR)
	$(AS) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.s=.lst)) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

# openocd -f interface/stlink.cfg -c "transport select hla_swd" -f "target/stm32l4x.cfg" -c "init;" -c "halt;"
# ou
# st-util -p 3333

debug: $(BUILD_DIR)/$(TARGET).elf
	#$(GDB) $< -tui -ex "target extended-remote :3333"
	$(GDB) $< -ex "target extended-remote :3333"
.PHONY: debug

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)
