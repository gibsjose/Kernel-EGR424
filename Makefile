# Generic Makefile template for ARMv7 Cortex M3 MCU
# Created by JJG

CC = arm-none-eabi-gcc
CFLAGS = -Wall -g -Os -march=armv7-m -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd -Wl,--gc-sections

SRC_DIR = .
BIN_DIR = .

# StellarisWare ek-lm3s6965 Driver Directory
DRV_DIR = C\:/StellarisWare/boards/ek-lm3s6965/drivers
UTILS_DIR = C\:/StellarisWare/utils

INCLUDES = -IC\:/StellarisWare -IC\:/StellarisWare/boards/ek-lm3s6965
LINKPATHS = -LC\:/StellarisWare/driverlib/gcc-cm3 -LC\:/Program\ Files\ \(x86\)/CodeSourcery/Sourcery\ G++\ Lite/arm-none-eabi/lib/thumb2/

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/scheduler.c $(SRC_DIR)/threads.c $(SRC_DIR)/timer_driver.c $(SRC_DIR)/oled_driver.c $(SRC_DIR)/startup_gcc.c $(SRC_DIR)/syscalls.c $(DRV_DIR)/rit128x96x4.c
LIBS = -ldriver-cm3
OBJS = $(SOURCES:.c=.o)

TARGETNAME = kernel
ELF = $(BIN_DIR)/$(TARGETNAME).elf
BIN = $(BIN_DIR)/$(TARGETNAME).bin

MAP = crsched.map
LS = $(SRC_DIR)/linkscript.x

TARGET = $(BIN)

all: $(TARGET)

%.o: %.c
	@echo "Building $@... "
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
	@echo "Done"
	@echo " "

$(ELF): $(OBJS) create.S
	@echo "Building $@... "
	$(CC) $(CFLAGS) $(INCLUDES) -o $(ELF) -T$(LS) $(LINKPATHS) -Wl,-Map,$(MAP) -Wl,--entry=ResetISR create.S $(OBJS) $(LIBS)
	@echo "Done"
	@echo " "

$(TARGET): $(ELF) $(OBJS)
	@echo "Building $@... "
	arm-none-eabi-objcopy -O binary $(TARGETNAME).elf $(TARGET)
	@cp -f $(TARGET) "/cygdrive/c/Users/$(USER)/Desktop"
	@echo "Done"
	@echo " "

clean:
	rm -f $(TARGET) $(BIN) $(ELF) $(OBJS)
