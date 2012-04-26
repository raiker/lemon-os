;extern debug_print, os_hang

section .data

error_de:
	db "Division by zero\0"

error_null:
	db "Unknown interrupt\0"

section .text

global isr_null, isr_de, isr_nmi, isr_bp

os_hang:
	hlt
	jmp os_hang

debug_print:
	ret

; null interrupt: should not be called

isr_null:
	mov eax, error_null
	push eax
	call debug_print
	call os_hang

; interrupt 0: divide-by-zero

isr_de:
	mov eax, error_de
	push eax
	call debug_print
	call os_hang

; interrupt 2: NMI

isr_nmi:
	; do nothing
	iret

; interrupt 3: breakpoint

isr_bp:
	; do nothing
	iret
