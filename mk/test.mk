QEMU_STM32 ?= ../qemu_stm32/arm-softmmu/qemu-system-arm
TEST_OUTDIR = $(OUTDIR)/test_out
TEST_INDIR = test
TEST_BUILDDIR = $(OUTDIR)/test

TEST_CASES = test-strlen \
			test-strcpy \
			test-strcmp \
			test-strncmp \
			test-cmdtok \
			test-itoa \
			test-find_events \
			test-find_envvar \
			test-fill_arg \
			test-export_envvar

TEST_CASE_TXTS = $(foreach case,$(TEST_CASES),$(TEST_OUTDIR)/$(case).out)

.PRECIOUS: $(TEST_BUILDDIR)/%.in

$(TEST_BUILDDIR)/%.in: $(TEST_INDIR)/%.in $(TEST_INDIR)/qemudbg_init.in
	@mkdir -p $(dir $@)
	@echo 'file '$(OUTDIR)/$(TARGET).elf > $@
	@echo 'set logging file' $(patsubst $(TEST_INDIR)/%.in,$(TEST_OUTDIR)/%.out,$<) >> $@
	@cat $(TEST_INDIR)/qemudbg_init.in >> $@
	@cat $< >> $@
	@echo 'c' >> $@

full_test:$(TEST_CASE_TXTS)

check: $(OUTDIR)/$(TARGET).bin
	$(QEMU_STM32) -M stm32-p103 \
		-gdb tcp::3333 -S \
		-serial stdio \
		-kernel $< -monitor null >/dev/null &
	make full_test
	@pkill -9 $(notdir $(QEMU_STM32))
	
$(TEST_OUTDIR)/%.out: $(TEST_BUILDDIR)/%.in
	@mkdir -p $(dir $@)
	$(CROSS_COMPILE)gdb -batch -x $<



