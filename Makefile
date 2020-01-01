BINS = emu-rv32i test1

CROSS_COMPILE = riscv-none-embed-
RV32I_CFLAGS = -march=rv32i -mabi=ilp32 -O3 -nostdlib

CFLAGS = -O3 -Wall
LDFLAGS = -lelf

RUN_TARGET := ./emu-rv32i
WORK_DIR = work/rv32imc


all: $(BINS)

emu-rv32i: emu-rv32i.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test1: test1.c
	$(CROSS_COMPILE)gcc $(RV32I_CFLAGS) -o $@ $<


%.log: $(WORK_DIR)/%.elf
	$(RUN_TARGET) $< +signature=sig.txt

check: $(BINS)
	./emu-rv32i test1

clean:
	$(RM) $(BINS)
