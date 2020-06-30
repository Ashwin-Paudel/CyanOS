MKDIR   := mkdir -p
RMDIR   := rm -rf
QEMU	:= qemu-system-i386
QMFLAGS := -S -gdb tcp::1234

BUILD	:= $(CURDIR)/build
BIN     := $(BUILD)/bin
OUT		:= $(BIN)/kernel.out
IMG		:= $(BIN)/kernel.img

.PHONY: all run debug clean kernel compile

all: compile


debug: compile
	$(QEMU) -kernel $(IMG) $(QMFLAGS)


run: compile
	$(QEMU) -kernel $(IMG)

clean:
	$(RMDIR) "$(BUILD)"

compile: kernel
	#python utils/create_ramdisk.py $(BUILD)

kernel: | $(BIN)
	$(MAKE) OBJ=$(BUILD)/obj/kernel OUT=$(OUT) IMG=$(IMG) -C "./kernel"
	python utils/create_ramdisk.py ./build

$(BIN):
	$(MKDIR) $@