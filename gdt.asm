; GDT entry builder

%macro MAKE_GDT_ENTRY 4 ; base, limit, flags, access
	%define %%access ((%4) | 0b00010000)
	%define %%flags ((%3) & 0b1100)
	dd \
		(((%1) & 0xffff) << 16) | \
		(((%2) & 0xffff) << 0)
	dd \
		(((%1) & 0xff000000) << 0) | \
		((%%flags & 0xf) << 20) | \
		(((%2) & 0xf0000) << 0) | \
		((%%access & 0xff) << 8) | \
		(((%1) & 0xff0000) >> 16)
%endmacro

; GDT entry flags

%define GDT_ACCESS_PR 0x80
%define GDT_ACCESS_PRIVL(x) (((x) & 0x3) << 5)
%define GDT_ACCESS_EX 0x8
%define GDT_ACCESS_DC 0x4
%define GDT_ACCESS_RW 0x2
%define GDT_ACCESS_AC 0x1

%define GDT_FLAGS_GR 0x8
%define GDT_FLAGS_SZ 0x4

%define GET_GDT_SELECTOR(x) ((x) - GDT_start)

; IDT entry builder

%macro MAKE_IDT_ENTRY 3 ; offset, cs_selector, type_attr
	dd \
		(((%2) & 0xffff) << 16) | \
		(((%1) & 0xffff))
	dd \
		(((%1) & 0xffff0000)) | \
		(((%3) & 0xff) << 8)
%endmacro

; IDT entry flags

%define IDT_TA_P 0x80
%define IDT_TA_DPL(x) (((x) & 0x3) << 5)
%define IDT_TA_TYP(x) (((x) & 0xf) | 0x8)
%define IDT_TA_INTERRUPT_GATE 0b1110
%define IDT_TA_TRAP_GATE 0b1111

%define MAKE_IDT_TRAP(x) MAKE_IDT_ENTRY (x), IDT_TA_P | IDT_TA_DPL(0) | IDT_TA_TRAP_GATE, GET_GDT_SELECTOR(GDT_code_entry)
%define MAKE_IDT_INTERRUPT(x) MAKE_IDT_ENTRY (x), IDT_TA_P | IDT_TA_DPL(0) | IDT_TA_INTERRUPT_GATE, GET_GDT_SELECTOR(GDT_code_entry)

; Global symbols

global load_gdt
extern isr_null, isr_de, isr_nmi, isr_bp
section .data

; The actual GDT

GDT_start:
	MAKE_GDT_ENTRY 0,0,0,0 ; ignored
GDT_code_entry:
	MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_EX
GDT_data_entry:
	MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_RW
GDT_end:

; The IDT

IDT_start:
; interrupt 0: divide-by-zero
;	MAKE_IDT_TRAP(isr_de)
; interrupt 1: debug exception
;	MAKE_IDT_TRAP(isr_null)
; interrupt 2: NMI
;	MAKE_IDT_INTERRUPT(isr_nmi)
; interrupt 3: breakpoint
;	MAKE_IDT_TRAP(isr_bp)
; interrupt 4: overflow
;	MAKE_IDT_TRAP(isr_null)
; interrupt 5: bound range exceeded
;	MAKE_IDT_TRAP(isr_null)
; interrupt 6: invalid opcode
;	MAKE_IDT_TRAP(isr_null)
; interrupt 7: device not available
;	MAKE_IDT_TRAP(isr_null)
; interrupt 8: double fault
;	MAKE_IDT_TRAP(isr_null)
; interrupt 9: coprocessor segment overrun (not generated any more)
;	MAKE_IDT_TRAP(isr_null)
; interrupt 10: invalid TSS
;	MAKE_IDT_TRAP(isr_null)
; interrupt 11: segment not present
;	MAKE_IDT_TRAP(isr_null)
; interrupt 12: stack fault
;	MAKE_IDT_TRAP(isr_null)
; interrupt 13: general protection fault
;	MAKE_IDT_TRAP(isr_null)
; interrupt 14: page fault
;	MAKE_IDT_TRAP(isr_null)
; interrupt 15: (undocumented)
;	MAKE_IDT_TRAP(isr_null)
; interrupt 16: floating-point error
;	MAKE_IDT_TRAP(isr_null)
; interrupt 17: alignment check exception
;	MAKE_IDT_TRAP(isr_null)
; interrupt 18: machine check exception
;	MAKE_IDT_TRAP(isr_null)
; interrupt 19: SIMD floating-point error
;	MAKE_IDT_TRAP(isr_null)
; interrupts 20-31: (reserved)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
;	MAKE_IDT_TRAP(isr_null)
IDT_end:
	
; GDT load structure

gdt_r:
.size:
	dw GDT_end - GDT_start ; size
.offset:
	dd GDT_start ; offset

; IDT load structure


section .text

; GDT loader function

load_gdt: ; load_gdt()
	mov eax, [esp + 4] ; load gdt address
	mov [gdt_r.offset], eax ; store it in global memory

	mov ax, [esp + 8] ; load gdt size
	mov [gdt_r.size], ax ; store

	lgdt [gdt_r]
	ret


