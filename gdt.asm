%macro MAKE_GDT_ENTRY 4 ; base, limit, flags, access
	%define %%access ((%4) | 0b00010000)
	%define %%flags ((%3) & 0b1100)
	dd \
		((%1 & 0xffff) << 16) | \
		((%2 & 0xffff) << 0)
	dd \
		((%1 & 0xff000000) << 0) | \
		((%3 & 0xf) << 20) | \
		((%2 & 0xf0000) << 0) | \
		((%%access & 0xff) << 8) | \
		((%1 & 0xff0000) >> 16)
%endmacro

%define GDT_ACCESS_PR 0x80
%define GDT_ACCESS_PRIVL(x) ((x & 0x3) << 5)
%define GDT_ACCESS_EX 0x8
%define GDT_ACCESS_DC 0x4
%define GDT_ACCESS_RW 0x2
%define GDT_ACCESS_AC 0x1

%define GDT_FLAGS_GR 0x8
%define GDT_FLAGS_SZ 0x4

global load_gdt

section .data

GDT_start:
MAKE_GDT_ENTRY 0,0,0,0 ; ignored
MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_EX
MAKE_GDT_ENTRY 0x00000000, 0xfffff, GDT_FLAGS_GR | GDT_FLAGS_SZ, GDT_ACCESS_PR | GDT_ACCESS_PRIVL(0) | GDT_ACCESS_RW
GDT_end:

gdt_r:
.size:
	dw GDT_end - GDT_start ; size
.offset:
	dd GDT_start ; offset

section .text

load_gdt: ; load_gdt()
	mov eax, [esp + 4] ; load gdt address
	mov [gdt_r.offset], eax ; store it in global memory

	mov ax, [esp + 8] ; load gdt size
	mov [gdt_r.size], ax ; store

	lgdt [gdt_r]
	ret


