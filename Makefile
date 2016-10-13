#Makefile for Imperial Defense game

TOOLCHAIN = arm-linux-gnueabi
arm-none-eabi: TOOLCHAIN = arm-none-eabi

TOOLPARAMS = -mfpu=vfp -mfloat-abi=hard -march=armv6zk
CC = gcc
CFLAGS = -std=gnu99 -Wall -O2

#temporary directory to hold object files
BUILD_DIR = ./build/

#output files
TARGET = ./kernel.img
LIST = ./kernel.list
MAP = ./kernel.map

#linker script
LINKER = ./kernel.ld

LIBS = -Llib/uspi/lib/ -luspi -Llib/uspi/env/lib/ -luspienv
INCLUDES = -Ilib/uspi/include -Ilib/uspi/env/include

#best tool in the world
IMG_CONVERT_SRC = ./imageconvert/imgconvert.c
IMG_CONVERT_OUT = ./imageconvert/imgconvert

#objects
OBJECT_FILES := $(patsubst %.s, $(BUILD_DIR)%.o, $(wildcard *.s))
OBJECT_FILES += $(patsubst %.c, $(BUILD_DIR)%.o, $(wildcard *.c))
 
all: extension

extension: uspi $(IMG_CONVERT_OUT) $(TARGET) $(LIST)

$(LIST) : $(BUILD_DIR)output.elf
	$(TOOLCHAIN)-objdump -d $(BUILD_DIR)output.elf > $(LIST)

$(TARGET) : $(BUILD_DIR)output.elf
	$(TOOLCHAIN)-objcopy $(BUILD_DIR)output.elf -O binary $(TARGET)

$(BUILD_DIR)output.elf : $(OBJECT_FILES) $(LINKER)
	$(TOOLCHAIN)-ld --no-undefined lib/uspi/env/lib/startup.o $(OBJECT_FILES) $(LIBS) -Map $(MAP) -o $(BUILD_DIR)output.elf -T $(LINKER)

$(BUILD_DIR)%.o: %.s $(BUILD_DIR)
	$(TOOLCHAIN)-as $(TOOLPARAMS) $< -o $@

$(BUILD_DIR)%.o: %.c $(BUILD_DIR)
	$(TOOLCHAIN)-gcc $(TOOLPARAMS) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR):
	mkdir $@

$(IMG_CONVERT_OUT) : $(IMG_CONVERT_SRC)
	$(CC) $(CFLAGS) $(IMG_CONVERT_SRC) -o $(IMG_CONVERT_OUT)
	$(MAKE) -C ./images/ "all"

clean:
	$(MAKE) -C ./images/ "clean"
	$(MAKE) -C ./lib/uspi/lib/ "clean"
	$(MAKE) -C ./lib/uspi/env/lib/ "clean"
	-rm -f $(IMG_CONVERT_OUT)
	-rm -rf $(BUILD_DIR)
	-rm -f $(TARGET)
	-rm -f $(LIST)
	-rm -f $(MAP)


uspi: 
	$(MAKE) -C lib/uspi/lib/ "$(TOOLCHAIN)"
	$(MAKE) -C lib/uspi/env/lib/ "$(TOOLCHAIN)"

arm-none-eabi: all