BITS 64

;===============
;ENTRY POINT
;===============
global _start

;===============
;MAKE PRINTF EXTERN
;===============

;===============
;STACK UNINITIALISED MEMORY
;===============
SECTION .bss
EqMyPrnStrLen	equ 64
DbMyPrnStr: 	resb EqMyPrnStrLen

;===============
;READONLY MEMORY
;===============
SECTION .rodata
DbMyPrnHexMap:	db '0123456789ABCDEF'

DbMyPrnFmtList: db 'scoxdb%'
EqMyPrnFmtLen	equ $-DbMyPrnFmtList

DqMyPrnFncList:	dq my_prn_str, my_prn_chr, my_prn_oct, my_prn_hex, my_prn_dec, my_prn_bin, my_prn_pct

EqMaxCounter	equ 0x10000000

;===============
;READONLY MEMORY (FOR USER SPACE)
;===============
SECTION .rodata
DbMsgString:	db 'love',0
DbWatString:	db 'wow',0
DbFmtString:	db 'I %s %x %d%%%c%b%s',0xA,0

;===============
;CODE SECTION
;===============
SECTION .text
_start:
		push QWORD DbWatString		;arg6
		push QWORD 0xbeda		;arg5
		push QWORD '!'			;arg4
		push QWORD -100			;arg3
		push QWORD 3802			;arg2
		push QWORD DbMsgString		;arg1
		push QWORD DbFmtString		;format string (arg0)

		call my_printf			;call printf

		add rsp, 7*8			;stack cleanup by the caller

		mov rax, 0x3c			;exit
		mov rdi, 0x00			;with code 0
		syscall				;call system

		ret				;canary ret

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
		push rbp			;save old rbp value
		mov rbp, rsp			; move to parameters area
		add rbp, 16			; skip call address and rbp

		push rbx			;save old rbx value (Sys V)

		mov rbx, [rbp]			;get format string address
		add rbp, 8			; and skip it

		mov al, 0x00			;find '\0'
		mov rdi, rbx			; in format string
		mov rcx, EqMaxCounter		; set max iteration count
		repne scasb			; find string size
		
		mov rdx, EqMaxCounter		;calc format string size
		sub rdx, rcx			; save format string size

		mov rdi, rbx			;set current substring address
		mov rcx, rdx			;set current substring size
.printfLoop:
		mov al, '%'			;search for '%'
		repne scasb			; find substring size

		mov al, [rdi]			;save current character

		push rcx			;store affected registers
		push rdi

		mov r8, rax			;save rax

		mov rdx, rdi			;set size
		sub rdx, rbx			;calc current size
		dec rdx				;skip % symb
		lea rsi, [rbx]			;set current address
		mov rdi, 0x01			;set output stream
		mov rax, 0x01			;call sys_write
		syscall				;write fmt string

		mov rax, r8			;get rax

		mov rcx, EqMyPrnFmtLen + 1	;pass format list len
		lea rdi, [DbMyPrnFmtList]	;search for al in format list
		repne scasb			;loop until hit

		cmp rcx, 0x00			;check if no such format
		je .printfLoopEnd		;and skip call in this case
		
		mov rbx, EqMyPrnFmtLen		;format index = format len
		sub rbx, rcx			;format index -= rcx
		call QWORD [DqMyPrnFncList + rbx*8]	;call format handler

.printfLoopEnd:
		pop rdi				;restore affected registers
		inc rdi				;skip current char
		mov rbx, rdi			;update substr start pos

		pop rcx				;load rcx
		cmp rcx, 0			;check for end of string

		jne .printfLoop			;loop on format string

		pop rbx				;restore old rbx value
		pop rbp				;restore old rbp value
		;TODO: to pop all the rest
		ret				;return
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
		mov rbx, [rbp]			;get string address
		add rbp, 8			; and skip it

		mov al, 0x00			;search for end of string
		mov rdi, rbx			; in argument string
		mov rcx, EqMaxCounter		;max iteration count
		repne scasb			; find string size

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01			; to stdout
		mov rsi, rbx			;from parameter string
		mov rdx, EqMaxCounter		;calculate length
		sub rdx, rcx			; using rcx value
		syscall				;call system

		ret				;return 
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
		mov rax, [rbp]			;get character value
		add rbp, 8			; and skip it

		mov [DbMyPrnStr], al		;move lower byte to output

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01			; to stdout
		lea rsi, [DbMyPrnStr]		;buffer to write
		mov rdx, 0x1			;length = 1
		syscall				;call system

		ret				;return
;===============
;my_prn_chr ENDS
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
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it
		mov r8, rax			;save rax

		cmp r8, 0			;compare to 0
		jge .skip_negation		;check if num < 0

		neg rax				;negate if num < 0

.skip_negation:
		mov ebx, 10			;set base
		mov rcx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		xor dl, dl			;dl = 0
		div ebx				;move to next digit

		add dl, '0'			;set dl to digit character
		mov [DbMyPrnStr + rcx], dl	;print digit

		dec rcx				;shift left to next index
		cmp eax, 0			;check for end of num
		jne .prnLoop			;continue if num != 0

		cmp r8, 0			;comare num to 0
		jge .skip_minus			;print neg prefix if num < 0

		dec rcx				;shift left
		mov BYTE [DbMyPrnStr + rcx + 1], '-'	;print neg prefix

.skip_minus:
		mov rbx, rcx			;set prefix len
		call my_prn_suff_to_out		;print buffer to stdout

		ret				;return
;===============
;my_prn_dec ENDS
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
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		mov cl, 4			;set log2 base = 4
		call my_prn_pow2_to_str		;call pow2 print helper

		dec rbx				;shift left
		dec rbx				; 2 times
		mov WORD [DbMyPrnStr + rbx + 1], '0x'	;print hex prefix

		call my_prn_suff_to_out		;print buffer to stdout

		ret
;===============
;my_prn_hex ENDS
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
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		mov cl, 3			;set log2 base = 3
		call my_prn_pow2_to_str		;call pow2 print helper

		dec rbx				;move to next digit index
		mov BYTE [DbMyPrnStr + rbx + 1], '0'	;print oct prefix

		call my_prn_suff_to_out		;print buffer to stdout

		ret				;return
;===============
;my_prn_oct ENDS
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
		mov rax, [rbp]			;get 32-bit val
		add rbp, 8			; and skip it

		mov cl, 1			;set log2 base = 1
		call my_prn_pow2_to_str		;call pow2 print helper

		dec rbx				;shift left
		dec rbx				; 2 times
		mov WORD [DbMyPrnStr + rbx + 1], '0b'	;print bin prefix

		call my_prn_suff_to_out		;print buffer to stdout

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
		mov BYTE [DbMyPrnStr], '%'	;set byte

		mov rax, 0x01			;call sys_write
		mov rdi, 0x01			;to stdout
		lea rsi, [DbMyPrnStr]		;from buffer
		mov rdx, 1			;length = 1
		syscall				;call system

		ret				;return
;===============
;my_prn_pct ENDS
;===============

;===============
;my_prn_pow2_to_str
;===============
;LAYOUT:
;	RAX - input number
;	CL = log2 base
;
;RETURN:
;	RBX - offset from DbMyPrnStr
;
;AFFECT:
;	RAX, RBX, RCX, RDX
;
;===============
my_prn_pow2_to_str:
		mov ch, 0x1			;bh = 1
		shl ch, cl			;bh = 2^bl
		dec ch				;bh = 2^bl - 1

		xor rdx, rdx			;rdx = 0
		mov rbx, EqMyPrnStrLen - 1	;set current byte pos
.prnLoop:
		mov dl, al			;get lower byte
		and dl, ch			;get digit
		shr eax, cl			;shrink to next digit

		mov dl, [DbMyPrnHexMap + rdx]	;get symbol representation
		mov [DbMyPrnStr + rbx], dl	;print received character

		dec rbx				;move to next digit index
		cmp eax, 0			;check for end of number
		jne .prnLoop			;continue if number != 0

		ret				;return
;===============
;my_prn_pow2_to_str ENDS
;===============

;===============
;my_prn_suff_to_out
;===============
;LAYOUT:
;	RBX - prefix length
;
;AFFECT:
;	RAX, RDX, RDI, RSI
;
;===============
my_prn_suff_to_out:
		mov rax, 0x01			;call sys_write
		mov rdi, 0x01			;to stdout
		lea rsi, [DbMyPrnStr + rbx + 1]	;from current digit position
		mov rdx, EqMyPrnStrLen		;calculate length
		sub rdx, rbx			;with rcx
		syscall				;call system

		ret				;return

;===============
;my_prn_suff_to_out ENDS
;===============
