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

DbMsgString:
		db 'fuck all this shit!',0xa
EqMsgLength	equ $-DbMsgString

SECTION .text
_start:
		mov rax, 0x01
		mov rdi, 0x01
		lea rsi, [DdDfaEdgeMap] 
		mov rdx, [DdDfaSize]
		shl rdx, 2
		syscall

		call custom_printf

		mov rax, 0x3c
		mov rdi, 0x00
		syscall

custom_printf:
		mov rax, 0x01
		mov rdi, 0x01
		lea rsi, [DbMsgString] 
		mov rdx, EqMsgLength
		syscall

		ret
