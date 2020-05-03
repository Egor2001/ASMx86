BITS 64

global hash_func_extrn

SECTION .rodata
%include "CRC32_IEEE_REV.asm"

SECTION .text

;===============
;calc 8-bit crc32
;===============
;LAYOUT (SYS V):
;	RDI <- str; RSI <- len; 
;	hash -> RAX;
;===============
;CLOBBER:
;	RAX; RDI; RSI; RDX; RCX; R8;
;===============
hash_func_extrn:
		push rbp			;prologue
		mov rbp, rsp
		add rbp, 16			;skip |old RBP|RIP|

		mov eax, 0xffffffff		;hash <- ~0
		mov rcx, rsi			;cnt <- len
.crc_loop:
		xor edx, edx			;symb <- 0
		mov dl, BYTE [rel rdi]		;symb <- str[i]

		lea r8, [rel symb_hash_map]	;ptr <- &symb_hash_map[symb]
		mov edx, DWORD [r8 + rdx*4]	;symb <- *ptr
		xor edx, eax			;symb <- symb ^ hash

		shr eax, 8			;hash <- hash >> 8
		xor eax, edx			;hash <- hash ^ symb

		inc rdi				;i <- i+1
		loop .crc_loop			;cnt == 0 => break

		pop rbp				;epilogue
		ret
;===============
