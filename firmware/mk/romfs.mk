ROMDIR = $(DATDIR)/rom0
DAT += $(OUTDIR)/$(DATDIR)/rom0.o

$(OUTDIR)/$(ROMDIR).o: $(OUTDIR)/$(ROMDIR).bin
	@mkdir -p $(dir $@)
	@echo "    OBJCOPY "$@
	@echo '' | $(CROSS_COMPILE)gcc -x c - \
			-mfpu=fpv4-sp-d16 -mfloat-abi=hard \
			-c -o $@
	@$(CROSS_COMPILE)objcopy -j .ARM.attributes \
		--add-section .rom.data=$< $@ $@

$(OUTDIR)/$(ROMDIR).bin: $(ROMDIR) $(OUTDIR)/$(TOOLDIR)/mkromfs
	@mkdir -p $(dir $@)
	@echo "    MKROMFS "$@
	@$(OUTDIR)/$(TOOLDIR)/mkromfs -d $< -o $@

$(ROMDIR):
	@mkdir -p $@

$(OUTDIR)/%/mkromfs: %/mkromfs.c
	@mkdir -p $(dir $@)
	@echo "    CC      "$@
	@gcc -Wall -o $@ $^
