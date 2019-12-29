/* Modified by Nober <s0913768710@gmail.com>
    13-DEC-2019 - Define all CSR macro
*/

/*
RISCV emulator for the RV32I architecture
based on TinyEMU by Fabrice Bellard, see https://bellard.org/tinyemu/
stripped down for RV32I only, all "gotos" removed, and fixed some bugs for the compliance test
by Frank Buss, 2018

Requires libelf-dev:

sudo apt-get install libelf-dev


Compile it like this:

gcc -O3 -Wall -lelf emu-rv32i.c -o emu-rv32i


It is compatible to Spike for the command line arguments, which means you can run
the compliance test from https://github.com/riscv/riscv-compliance like this:

make RISCV_TARGET=spike RISCV_DEVICE=rv32i TARGET_SIM=/full/path/emulator variant

It is also compatible with qemu32, as it is used for Zephyr. You can compile the
Zephyr examples for qemu like this:

cd zephyr
source zephyr-env.sh
cd samples/synchronization
mkdir build && cd build
cmake -GNinja -DBOARD=qemu_riscv32 ..
ninja

After this you can run it with the emulator like this:

emu-rv32i zephyr/zephyr.elf


original copyright:
*/



/*
 * RISCV emulator
 *
 * Copyright (c) 2016 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>


// ====================================================== //
// =================== User Trap Setup ================== //
// ====================================================== //
uint32_t ustatus;           /* User status register  */
uint32_t uie;               /* User interrupt-enable register */
uint32_t utvec;             /* User trap handler base address */
// ====================================================== //
// ================= User Trap Handling ================= //
// ====================================================== //
uint32_t uscratch;           /* Scratch register for user trap handlers*/
uint32_t uepc;               /* User exception program counter */
uint32_t ucause;             /* User trap cause*/
uint32_t ubadaddr;           /* User bad address */
uint32_t uip;                /* User interrupt pending */
// ====================================================== //
// ============== User Floating-Point CSRs ============== //
// ====================================================== //
uint32_t fflags;             /* Floating-Point Accrued Exceptions*/
uint32_t frm;                /* Floating-Point Dynamic Rounding Mode*/
uint32_t fcsr;               /* Floating-Point Control and Status Register (frm + fflags )*/
// ====================================================== //
// ================ User Counter / TImers =============== //
// ====================================================== //
uint32_t cycle;              /* Cycle counter for RDCYCLE instruction */
uint32_t time;               /* Timer for RDTIME instruction */
uint32_t instret;            /* Instructions-retired counter for RDINSTRET instructions*/
uint32_t cycleh;             /* Upper 32 bits of cycle, RV32I only. */
uint32_t timeh;              /* Upper 32 bits of time, RV32I only. */


// ====================================================== //
// ================ Supervior Trap Setup ================ //
// ====================================================== //
uint32_t sstatus;            /* Supervisor status register */
uint32_t sedeleg;            /* Supervisor exception delegation register */
uint32_t sideleg;            /* Supervisor interrupt delegation register */
uint32_t sie;                /* Supervisor interrupt-enable register*/
uint32_t stvec;              /* Supervisor trap handler base address */
// ====================================================== //
// ============== Supervisor Trap Handling ============== //
// ====================================================== //
uint32_t sscratch;          /* Scratch register for supervisor trap handlers */
uint32_t sepc;              /* Supervisor exception program counter */
uint32_t scause;            /* Supervisor trap cause */
uint32_t sbadaddr;          /* Supervisor bad address */
uint32_t sip;               /* Supervisor interrupt pending */
// ====================================================== //
// ======== Supervisor Protection and Translation ======= //
// ====================================================== //
uint32_t sptbr;             /* Page-table base register */


// ====================================================== //
// ================ Hypervisor Trap Setup =============== //
// ====================================================== //
uint32_t hstatus;           /* Hypervisor status register */
uint32_t hedeleg;           /* Hypervisor exception delegation register */
uint32_t hideleg;           /* Hyperbisor interrupt delegation register */
uint32_t hie;               /* Hypervisor interrupt-enable register */
uint32_t htvec;             /* Hypervisor trap handler base address */
// ====================================================== //
// ============== Hypervisor Trap Handling ============== //
// ====================================================== //
uint32_t hscratch;           /* Scratch register for hypervisor trap handlers */
uint32_t hepc;               /* Hypervisor exception program counter */
uint32_t hcause;             /* Hypervisor trap cause */
uint32_t hbadaddr;           /* Hypervisor bad address */
uint32_t hip;                /* Hypervisor interrupt pending */
// ====================================================== //
// ======== Hypervisor Protection and Translation ======= //
// ====================================================== //
// uint32_t TBD;                /* TBD */


// ====================================================== //
// ============ Machine Information Register ============ //
// ====================================================== //
uint32_t mvendorid;           /* Vendor ID*/
uint32_t marchid;             /* Architecture ID */
uint32_t mimpid;              /* Implementation ID */
uint32_t mhartid;             /* Hardware thread ID */
// ====================================================== //
// ================= Machine Trap Setup ================= //
// ====================================================== //
uint32_t mstatus;             /* Machine status register */
uint32_t misa;                /* ISA and extensions */
uint32_t medeleg;             /* Machine exception delegation register */
uint32_t mideleg;             /* Machine interrupt delegation register */
uint32_t mie;                 /* Machine interrupt-enable register */
uint32_t mtvec;               /* Machine trap-handler base address */
// ====================================================== //
// ================ Machine Trap Handling =============== //
// ====================================================== //
uint32_t mscratch;            /* Scratch register for machine trap handlers */
uint32_t mepc;                /* Machine exception program counter */
uint32_t mcause;              /* Machine trap cause */
uint32_t mbadaddr;            /* Machine bad address */
uint32_t mip;                 /* Machine interrupt pending */
// ====================================================== //
// ========= Machine Protextion and Translation ========= //
// ====================================================== //
uint32_t mbase;               /* Base regsiter */
uint32_t mbound;              /* Bound register */
uint32_t mibase;              /* Instruction base register */
uint32_t mibound;             /* Instruction bound register */
uint32_t mdbase;              /* Data base register */
uint32_t mdbound;             /* Data bound register */