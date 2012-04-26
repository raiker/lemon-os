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

section .gdt

global GDT_start, GDT_code_entry, GDT_data_entry, GDT_end

; The actual GDT

align 8

GDT_start:
	MAKE_GDT_ENTRY 0,0,0,0 ; ignored
GDT_code_entry:
	MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_EX
GDT_data_entry:
	MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_RW
GDT_end:

section .idt.bss

global IDT_start, IDT_end

; Space for the IDT

align 8

IDT_start:
	times 256 dq 0
IDT_end:

section .data
; GDT load structure

gdt_r:
.size:	
	dw GDT_end - GDT_start - 1 ; size
.offset:
	dd GDT_start ; offset

; IDT load structure

idt_r:
.size:
	dw IDT_end - IDT_start - 1; size
.offset:
	dd IDT_start

section .text

global load_gdt, load_idt

; GDT loader function

load_gdt: ; load_gdt()
	lgdt [gdt_r]
	ret

load_idt: ; load_idt()
	lidt [idt_r]
	ret
