GDB=arm-none-eabi-gdb

gdbauto:
	$(GDB) -tui -x ./tool/gdbscript
