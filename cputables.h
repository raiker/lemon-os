#include <stdint.h>

extern "C" void load_gdt();
extern "C" void load_idt();

void build_idt();

//isrs
extern uintptr_t isr_null;
extern uintptr_t isr_de;
extern uintptr_t isr_nmi;
extern uintptr_t isr_bp;

//memory symbols
extern uintptr_t IDT_start, IDT_end, GDT_start, GDT_end;
extern uintptr_t GDT_code_entry;
extern uintptr_t GDT_data_entry;
