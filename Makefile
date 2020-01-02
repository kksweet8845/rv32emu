BINS = emu-rv32i test1
TEST_TARGETS = \
	C-ADD.elf \
	C-ADDI.elf \
	C-ADDI4SPN.elf \
	C-AND.elf \
	C-ANDI.elf \
	C-BEQZ.elf \
	C-BNEZ.elf \
	C-J.elf \
	C-JAL.elf \
	C-JALR.elf \
	C-JR.elf \
	C-LI.elf \
	C-LUI.elf \
	C-LW.elf \
	C-LWSP.elf \
	C-MV.elf \
	C-NOP.elf \
	C-OR.elf \
	C-SLLI.elf \
	C-SRAI.elf \
	C-SRLI.elf \
	C-SUB.elf \
	C-SW.elf \
	C-SWSP.elf \
	C-XOR.elf

CROSS_COMPILE = riscv-none-embed-
RV32I_CFLAGS = -march=rv32i -mabi=ilp32 -O3 -nostdlib

CFLAGS = -O3 -Wall
LDFLAGS = -lelf

RUN_TARGET := ./emu-rv32i
WORK_DIR = work/rv32imc






all: $(BINS)

check-all:
	make $(patsubst %.elf, %.log, $(TEST_TARGETS))

emu-rv32i: emu-rv32i.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

test1: test1.c
	$(CROSS_COMPILE)gcc $(RV32I_CFLAGS) -o $@ $<


%.log: $(WORK_DIR)/%.elf
	$(RUN_TARGET) --elf $< --signature$(patsubst %.elf, %.signature.output, $<) --output sig.txt

check: $(BINS)
	./emu-rv32i test1

clean:
	$(RM) $(BINS)
