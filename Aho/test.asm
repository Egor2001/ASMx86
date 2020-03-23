BITS 64

global _start
extern DdDfaSize, DdDfaTermMap, DdDfaLinkArr, DdDfaEdgeMap

SECTION .data
		;DFA automaton for string detection
DdDfaSize:
		dd 12
DdDfaTermMap:
		dd 0,0,0,1,0,0,1,0,0,1,0,1 
DdDfaLinkArr:
		dd 0,0,0,0,0,0,0,0,0,0,3,0 
DdDfaEdgeMap:
		dd 1,1,5,8,5,1,5,8,1,5,11,1 
		dd 4,2,4,4,4,6,4,4,9,4,4,9 
		dd 7,7,3,7,7,7,10,7,7,3,7,7 

DbMyPrnChr: 	db 0x00

SECTION .rodata
DbMsgString:
		db 'test str %d',0x0a,0x00
EqMsgLength	equ $-DbMsgString

SECTION .text
_start:
		mov rax, 0x01
		mov rdi, 0x01
		lea rsi, [DdDfaEdgeMap] 
		mov rdx, [DdDfaSize]
		shl rdx, 2
		syscall

		push 'a'
		push DbMsgString
		call my_printf
		add rsp, 16

		mov rax, 0x3c
		mov rdi, 0x00
		syscall

		ret

;===============
;LAYOUT:
;	RAX - string address
;	RSP - stack pointer
;===============
my_printf:
		;TODO: to push all the rest
		push rbp		;save old rbp value
		mov rbp, rsp		; move to parameters area
		add rbp, 16		; skip call address and old rbp value

		call my_prn_str		;simply print format string
		call my_prn_chr

		pop rbp			;restore old rbp value
		;TODO: to pop all the rest

		ret

my_prn_str:
		;TODO: to push all the rest
		mov rbx, [rbp]		;get format string address
		add rbp, 8		; and skip it

		mov al, 0x00
		mov rdi, rbx
		mov rcx, 0x10000000	;max iteration count
		repne scasb		; find string size

		mov rax, 0x01		;call sys_write
		mov rdi, 0x01
		mov rsi, rbx
		mov rdx, 0x10000000
		sub rdx, rcx
		syscall

		;TODO: to pop all the rest
		ret

my_prn_chr:
		;TODO: to push all the rest
		mov rbx, [rbp]		;get format string address
		add rbp, 8		; and skip it

		mov [DbMyPrnChr], bl	;move lower byte value to output data

		mov rax, 0x01		;call sys_write
		mov rdi, 0x01
		lea rsi, [DbMyPrnChr]
		mov rdx, 0x1
		syscall

		;TODO: to pop all the rest
		ret

DdMyPrnU32:	dd 0x00000000
my_prn_u32:

		ret
