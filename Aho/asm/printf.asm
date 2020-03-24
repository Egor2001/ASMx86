BITS 64

global _start
extern my_printf, my_prn_str, my_prn_chr, my_prn_hex

SECTION .bss
EqMyPrnStrLen	equ 64
DbMyPrnStr: 	resb EqMyPrnStrLen

SECTION .rodata
DbMyPrnHexMap:	db '0123456789ABCDEF'

DbMyPrnFmtList: db 'scoxdb%'
EqMyPrnFmtLen	equ $-DbMyPrnFmtList

DqMyPrnFncList:	dq my_prn_str, my_prn_chr, my_prn_oct, my_prn_hex, my_prn_dec, my_prn_bin, my_prn_pct

EqMaxCounter	equ 0x10000000

SECTION .rodata
DbMsgString:	db 'love',0
DbFmtString:	db 'I %s %o %d%%%c%b',0xA,0

SECTION .text
_start:
		push QWORD 0xbeda
		push QWORD '!'
		push QWORD -100
		push QWORD 3802
		push DbMsgString
		push DbFmtString

		call my_printf

		add rsp, 24

		mov rax, 0x3c
		mov rdi, 0x00
		syscall

		ret

;===============
;my_printf
;===============
;LAYOUT:
;	RAX - string address
;	RSP - stack pointer
;AFFECT:
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_printf:
		;TODO: to push all the rest
		push rbp		;save old rbp value
		mov rbp, rsp		; move to parameters area
		add rbp, 16		; skip call address and old rbp value

		push rbx		;save old rbx value (Sys V)

		mov rbx, [rbp]		;get format string address
		add rbp, 8		; and skip it

		mov al, 0x00		;find '\0'
		mov rdi, rbx		; in format string
		mov rcx, EqMaxCounter	; set max iteration count
		repne scasb		; find string size
		
		mov rdx, EqMaxCounter	;calc format string size
		sub rdx, rcx		; save format string size

		mov rdi, rbx		;set current substring address
		mov rcx, rdx		;set current substring size
.printfLoop:
		mov al, '%'		;search for '%'
		repne scasb		; find substring size

		mov al, [rdi]		;save current character

		push rcx		;store affected registers
		push rdi

		push rax

		mov rdx, rdi		;set size
		sub rdx, rbx		;calc current size
		dec rdx			;skip % symb
		lea rsi, [rbx]		;set current address
		mov rdi, 0x01		;set output stream
		mov rax, 0x01		;call sys_write
		syscall			;write fmt string

		pop rax

		mov rcx, EqMyPrnFmtLen + 1
		lea rdi, [DbMyPrnFmtList]
		repne scasb

		cmp rcx, 0x00
		je .printfLoopEnd
		
		mov rbx, EqMyPrnFmtLen
		sub rbx, rcx
		call QWORD [DqMyPrnFncList + rbx*8]

.printfLoopEnd:
		pop rdi			;restore affected registers
		inc rdi			;skip current char
		mov rbx, rdi		;update substr start pos

		pop rcx
		cmp rcx, 0

		jne .printfLoop		;loop until reaching end of string

		pop rbx			;restore old rbx value
		pop rbp			;restore old rbp value
					;TODO: to pop all the rest
		ret
;===============
;my_printf ENDS
;===============

;===============
;my_prn_str
;===============
;LAYOUT:
;	[RBP] - string address
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_str:
		;TODO: to push all the rest
		mov rbx, [rbp]		;get string address
		add rbp, 8		; and skip it

		mov al, 0x00
		mov rdi, rbx
		mov rcx, EqMaxCounter	;max iteration count
		repne scasb		; find string size

		mov rax, 0x01		;call sys_write
		mov rdi, 0x01
		mov rsi, rbx
		mov rdx, EqMaxCounter
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_str ENDS
;===============

;===============
;my_prn_chr
;===============
;LAYOUT:
;	[RBP] - char value
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_chr:
		;TODO: to push all the rest
		mov rax, [rbp]		;get character value
		add rbp, 8		; and skip it

		mov [DbMyPrnStr], al	;move lower byte value to output data

		mov rax, 0x01		;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr]
		mov rdx, 0x1
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_chr ENDS
;===============

;===============
;my_prn_oct
;===============
;LAYOUT:
;	[RBP] - char value
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_oct:
		;TODO: to push all the rest
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		xor rdx, rdx
		mov rcx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		mov dl, al
		and dl, 0x07
		shr eax, 3

		mov dl, [DbMyPrnHexMap + rdx]
		mov [DbMyPrnStr + rcx], dl

		dec rcx
		cmp eax, 0
		jne .prnLoop

		dec rcx
		mov BYTE [DbMyPrnStr + rcx + 1], '0'

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr + rcx + 1]
		mov rdx, EqMyPrnStrLen
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_oct ENDS
;===============

;===============
;my_prn_hex
;===============
;LAYOUT:
;	[RBP] - char value
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_hex:
		;TODO: to push all the rest
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		xor rdx, rdx
		mov rcx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		mov dl, al
		and dl, 0x0f
		shr eax, 4

		mov dl, [DbMyPrnHexMap + rdx]
		mov [DbMyPrnStr + rcx], dl

		dec rcx
		cmp eax, 0
		jne .prnLoop

		dec rcx
		dec rcx
		mov WORD [DbMyPrnStr + rcx + 1], '0x'

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr + rcx + 1]
		mov rdx, EqMyPrnStrLen
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_hex ENDS
;===============

;===============
;my_prn_dec
;===============
;LAYOUT:
;	[RBP] - char value
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_dec:
		;TODO: to push all the rest
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it
		mov r8, rax

		cmp r8, 0
		jge .skip_negation

		neg rax

.skip_negation:
		mov ebx, 10
		mov rcx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		xor dl, dl
		div ebx

		add dl, '0'
		mov [DbMyPrnStr + rcx], dl

		dec rcx
		cmp eax, 0
		jne .prnLoop

		cmp r8, 0
		jge .skip_minus

		dec rcx
		mov BYTE [DbMyPrnStr + rcx + 1], '-'

.skip_minus:
		mov rax, 0x01			;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr + rcx + 1]
		mov rdx, EqMyPrnStrLen
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_dec ENDS
;===============

;===============
;my_prn_bin
;===============
;LAYOUT:
;	[RBP] - char value
;	RSP - stack pointer
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_bin:
		;TODO: to push all the rest
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		mov rcx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		mov dl, al
		and dl, 0x01
		shr rax, 1

		add dl, '0'
		mov [DbMyPrnStr + rcx], dl

		dec rcx
		cmp rax, 0
		jne .prnLoop

		dec rcx
		dec rcx
		mov WORD [DbMyPrnStr + rcx + 1], '0b'

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr + rcx + 1]
		mov rdx, EqMyPrnStrLen
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret
;===============
;my_prn_bin ENDS
;===============

;===============
;my_prn_pct
;===============
;LAYOUT:
;
;AFFECT:
;	RBP,
;   	RDI, RSI,
;   	RAX, RBX, RCX, RDX
;===============
my_prn_pct:
		mov BYTE [DbMyPrnStr], '%'

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnStr]
		mov rdx, 1
		syscall

		ret
;===============
;my_prn_pct ENDS
;===============
