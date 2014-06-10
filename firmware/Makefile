TARGET = main
.DEFAULT_GOAL = all

CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
CFLAGS = -fno-common -ffreestanding -O0 \
			-fno-builtin \
         -gdwarf-2 -g3 -Wall -Werror \
         -mcpu=cortex-m4 -mthumb \
         -Wl,-Tmain.ld -nostartfiles \
         -DUSER_NAME=\"$(USER)\" \
         -DSTM32F429_439xx -DUSE_STDPERIPH_DRIVER -march=armv7e-m -mtune=cortex-m4 -mlittle-endian

ARCH = CM4
VENDOR = ST
PLAT = STM32F4xx

LIBDIR = .
CMSIS_LIB=$(LIBDIR)/libraries/CMSIS
STM32_LIB=$(LIBDIR)/libraries/STM32F4xx_StdPeriph_Driver
UTIL_LIB=$(LIBDIR)/libraries/Utilities

CMSIS_PLAT_SRC = $(CMSIS_LIB)/Device/$(VENDOR)/$(PLAT)


OUTDIR = build
SRCDIR = ./ \
	 OS_kernel/src \
         $(CMSIS_LIB)/CoreSupport \
         $(STM32_LIB)/src \
		 $(UTIL_LIB)/STM32F429I-Discovery \
         $(CMSIS_PLAT_SRC)/Source/Templates
INCDIR = ./ \
	 OS_kernel/include \
         $(CMSIS_LIB)/Include \
         $(STM32_LIB)/inc \
		 $(UTIL_LIB)/STM32F429I-Discovery \
         $(CMSIS_PLAT_SRC)/Include
INCLUDES = $(addprefix -I,$(INCDIR))
DATDIR = data
TOOLDIR = tool

SRC = $(wildcard $(addsuffix /*.c,$(SRCDIR))) \
      $(wildcard $(addsuffix /*.s,$(SRCDIR))) \
      $(CMSIS_PLAT_SRC)/Source/Templates/gcc_ride7/startup_stm32f429_439xx.s
OBJ := $(addprefix $(OUTDIR)/,$(patsubst %.s,%.o,$(SRC:.c=.o)))
DEP = $(OBJ:.o=.o.d)
DAT =

MAKDIR = mk
MAK = $(wildcard $(MAKDIR)/*.mk)

include $(MAK)

all: $(OUTDIR)/$(TARGET).bin $(OUTDIR)/$(TARGET).lst

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/$(TARGET).elf
	@echo "    OBJCOPY "$@
	@$(CROSS_COMPILE)objcopy -Obinary $< $@

$(OUTDIR)/$(TARGET).lst: $(OUTDIR)/$(TARGET).elf
	@echo "    LIST    "$@
	@$(CROSS_COMPILE)objdump -S $< > $@

$(OUTDIR)/$(TARGET).elf: $(OBJ) $(DAT)
	@echo "    LD      "$@
	@echo "    MAP     "$(OUTDIR)/$(TARGET).map
	@$(CROSS_COMPILE)gcc $(CFLAGS) -Wl,-Map=$(OUTDIR)/$(TARGET).map -o $@ $^

$(OUTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CROSS_COMPILE)gcc $(CFLAGS) -MMD -MF $@.d -o $@ -c $(INCLUDES) $<
	
$(OUTDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@$(CROSS_COMPILE)gcc $(CFLAGS) -MMD -MF $@.d -o $@ -c $(INCLUDES) $<

flash:$(OUTDIR)/$(TARGET).bin
	st-flash write $(OUTDIR)/$(TARGET).bin 0x8000000

clean:
	rm -rf $(OUTDIR)

-include $(DEP)
