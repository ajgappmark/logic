F_CPU = 8000000

CFLAGS += -W -Wall --std=gnu99 -Os
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums
CFLAGS += -fno-diagnostics-show-caret
CFLAGS += -mmcu=attiny45
CFLAGS += -Wl,--gc-sections
CFLAGS += -DF_CPU=$(F_CPU)

CFLAGS_APP = $(CFLAGS)
CFLAGS_APP += -Wl,--section-start=.text=0x400

CFLAGS_BOOT = $(CFLAGS)
CFLAGS_BOOT += -nostartfiles

APP_SRC = \
  leds.h \
  logic_probe.c

BOOT_SRC = \
  leds.h \
  bootloader.c \
  startup.S \
  soft_uart.S

all: bootloader.elf logic_probe.elf

bootloader.elf: $(BOOT_SRC)
	@echo CC $@
	@avr-gcc $(CFLAGS_BOOT) $^ -o bootloader.elf
	@echo size:
	@avr-size -t bootloader.elf

logic_probe.elf:  $(APP_SRC)
	@echo CC $@
	@avr-gcc $(CFLAGS_APP) $^ -o logic_probe.elf
	@echo OBJCOPY $@
	@avr-objcopy -O binary logic_probe.elf logic_probe.bin
	@echo size:
	@avr-size -t logic_probe.elf

prog: bootloader.elf
	avrdude -v -c jtag3isp -p t45 -P usb -e -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe2:m -U flash:w:bootloader.elf

prog_app: logic_probe.elf
	avrdude -v -c jtag3isp -p t45 -P usb -e -U efuse:w:0xfe:m -U hfuse:w:0xdd:m -U lfuse:w:0xe2:m -U flash:w:logic_probe.elf

read:
	avrdude -v -c jtag3isp -p t45 -P usb -U flash:r:/tmp/debug.bin:r

clean:
	@echo CLEAN
	@-rm -rf bootloader.elf logic_probe.elf logic_probe.bin

