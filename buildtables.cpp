#include "include/common/stdint.h"
#include "cputables.h"

struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_high;
};

static idt_entry *IDT = (idt_entry*)IDT_start;

static inline idt_entry build_idt_entry(uint32_t offset, uint16_t selector, uint8_t type_attr){
	idt_entry retval;
	retval.offset_low = offset & 0xffff;
	retval.selector = selector;
	retval.type_attr = type_attr;
	retval.offset_high = offset >> 16;
	
	return retval;
}

#define GET_GDT_SELECTOR(x) ((x) - GDT_start)

#define IDT_TA_P 0x80
#define IDT_TA_DPL(x) (((x) & 0x3) << 5)
#define IDT_TA_TYP(x) (((x) & 0xf) | 0x8)
#define IDT_TA_INTERRUPT_GATE 0b1110
#define IDT_TA_TRAP_GATE 0b1111

#define MAKE_IDT_TRAP(x) build_idt_entry(x, GET_GDT_SELECTOR(GDT_code_entry), IDT_TA_P | IDT_TA_DPL(0) | IDT_TA_TRAP_GATE)
#define MAKE_IDT_INTERRUPT(x) build_idt_entry(x, GET_GDT_SELECTOR(GDT_code_entry), IDT_TA_P | IDT_TA_DPL(0) | IDT_TA_INTERRUPT_GATE)

void build_idt(){
// interrupt 0: divide-by-zero
	IDT[0x00] = MAKE_IDT_TRAP(isr_de);
// interrupt 1: debug exception
	IDT[0x01] = MAKE_IDT_TRAP(isr_null);
// interrupt 2: NMI
	IDT[0x02] = MAKE_IDT_INTERRUPT(isr_nmi);
// interrupt 3: breakpoint
	IDT[0x03] = MAKE_IDT_TRAP(isr_bp);
// interrupt 4: overflow
	IDT[0x04] = MAKE_IDT_TRAP(isr_null);
// interrupt 5: bound range exceeded
	IDT[0x05] = MAKE_IDT_TRAP(isr_null);
// interrupt 6: invalid opcode
	IDT[0x06] = MAKE_IDT_TRAP(isr_null);
// interrupt 7: device not available
	IDT[0x07] = MAKE_IDT_TRAP(isr_null);
// interrupt 8: double fault
	IDT[0x08] = MAKE_IDT_TRAP(isr_null);
// interrupt 9: coprocessor segment overrun (not generated any more)
	IDT[0x09] = MAKE_IDT_TRAP(isr_null);
// interrupt 10: invalid TSS
	IDT[0x0a] = MAKE_IDT_TRAP(isr_null);
// interrupt 11: segment not present
	IDT[0x0b] = MAKE_IDT_TRAP(isr_null);
// interrupt 12: stack fault
	IDT[0x0c] = MAKE_IDT_TRAP(isr_null);
// interrupt 13: general protection fault
	IDT[0x0d] = MAKE_IDT_TRAP(isr_null);
// interrupt 14: page fault
	IDT[0x0e] = MAKE_IDT_TRAP(isr_null);
// interrupt 15: (undocumented)
	IDT[0x0f] = MAKE_IDT_TRAP(isr_null);
// interrupt 16: floating-point error
	IDT[0x10] = MAKE_IDT_TRAP(isr_null);
// interrupt 17: alignment check exception
	IDT[0x11] = MAKE_IDT_TRAP(isr_null);
// interrupt 18: machine check exception
	IDT[0x12] = MAKE_IDT_TRAP(isr_null);
// interrupt 19: SIMD floating-point error
	IDT[0x13] = MAKE_IDT_TRAP(isr_null);
// interrupts 20-31: (reserved)
	IDT[0x14] = MAKE_IDT_TRAP(isr_null);
	IDT[0x15] = MAKE_IDT_TRAP(isr_null);
	IDT[0x16] = MAKE_IDT_TRAP(isr_null);
	IDT[0x17] = MAKE_IDT_TRAP(isr_null);
	IDT[0x18] = MAKE_IDT_TRAP(isr_null);
	IDT[0x19] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1a] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1b] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1c] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1d] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1e] = MAKE_IDT_TRAP(isr_null);
	IDT[0x1f] = MAKE_IDT_TRAP(isr_null);
	
	load_idt();
}
