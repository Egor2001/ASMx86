BITS 64

global _start

SECTION .rodata
%include "CRC32_IEEE_REV.asm"

msg:		db 'fuckfuckfuck',10,0
msg_len		equ $ - msg - 1
byte_hash_map:	dd 122, 100

SECTION .text
_start:
		mov rdi, msg			;pass string
		mov rsi, msg_len		;pass string
		call crc32_hash			;call hash

		mov rax, 0x3c			;exit
		xor rdi, rdi			;code 0
		syscall

		ret				;canary

;===============
;calc 8-bit crc32
;===============
;LAYOUT (SYS V):
;	RDI <- str; RSI <- len; 
;	hash -> RAX;
;===============
;CLOBBER:
;	RAX; RDI; RSI; RDX; R8;
;===============
crc32_hash:
		push rbp			;prologue
		mov rbp, rsp
		add rbp, 16			;skip |old RBP|RIP|

		mov eax, 0xffffffff		;hash <- ~0
.crc_loop:
		xor edx, edx			;symb <- 0
		mov dl, BYTE [rel rdi]		;symb <- str[i]

		lea r8, [symb_hash_map + rdx*4]	;ptr <- &symb_hash_map[symb]
		mov edx, DWORD [rel r8]		;symb <- *ptr
		xor edx, eax			;symb <- symb ^ hash

		shr eax, 8			;hash <- hash >> 8
		xor eax, edx			;hash <- hash ^ symb

		inc rdi				;i <- i+1
		cmp BYTE [rel rdi], 0		;str[i] == 0 =>
		jne .crc_loop			; => break

		pop rbp				;epilogue
		ret
;===============
