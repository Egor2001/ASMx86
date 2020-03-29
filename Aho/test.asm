BITS 64

;===============
;include macros
;===============
%include "macro.asm"
;===============

;===============
;include macros
;===============
%include "dfa.asm"
;===============

global _start

SECTION .rodata
DbAhoMsg:	db 'detected term: ',0xA,0
EqAhoMsgLen:	equ ($ - DbAhoMsg)

DbAhoHexMap:	db '0123456789abcdef'

SECTION .data
DbAhoBuf:	db 0,0,0,0,0,0,0,0,0x0A
EqAhoBufLen:	equ ($ - DbAhoBuf) - 1

;DbSrcStr:	db 'abacababca',0
DbSrcStr:	db 'babcab',0

SECTION .text
;===============
;PROC main
;===============
_start:
		mov rax, 0xDED
		call aho_print_term

		lea rax, [DbSrcStr]
		call aho_parse_str

		mov rax, 0x3c
		mov rdi, 0
		syscall

		ret
;===============
;ENDP main
;===============

;===============
;PROC aho_parse_str
;	Parses input string and prints all pattern matches
;===============
;LAYOUT:
;	RAX: string address
;AFFECT:
;	RDI, RSI, RAX, RBX, RCX
;===============
aho_parse_str:
		sysv_prologue			;sysV64 convention

		mov rdi, rax			;str_addr

		mov rcx, 0
		mov rsi, 0			;state_idx = 0
.parse_loop:
		xor rax, rax			;cleanup before calc idx
		mov al, [rdi]			; = symb
		sub al, 'a'			;get index
		mul DWORD [DdDfaSize]		; *= DFA_SIZE
		add rax, rsi			; += state_idx
		shl rax, 2			; *= sizeof(DWORD)

		mov esi, [DdDfaEdgeMap + rax]	;state_idx = edge_map[rdx]
		mov ebx, esi
.search_loop:
		cmp DWORD [DdDfaTermMap + ebx*4], 0	;check if term
		je .search_next				; and stop if not

		multipush rdi, rsi, rcx		;store affected registers

		mov rax, rcx			;pass current position
		call aho_print_term		;print message

		multipop rdi, rsi, rcx		;load affected registers
.search_next:
		mov ebx, [DdDfaLinkArr + ebx*4]	;go to next link
		cmp ebx, 0			; and check for link to 0
		jne .search_loop		;proc next link or stop if 0

		inc rcx				;++cnt
		inc rdi				;++pos
		cmp BYTE [rdi], 0		; and check for '\0' symb
		jne .parse_loop			;proc next state or stop if 0

		sysv_epilogue			;sysV64 convention
		ret				;return
;===============
;ENDP aho_parse_str
;===============

;===============
;PROC aho_print_term
;	Prints end position of detected term
;===============
;LAYOUT:
;	RAX: state number
;AFFECT:
;	RDI, RSI, RAX, RCX, RDX
;===============
aho_print_term:
		push rbp
		mov rbp, rsp
		push rbx

		push rax			;store index

		mov rax, 0x1			;system write
		mov rdi, 0x1			; to stdout
		lea rsi, [DbAhoMsg]		;info str
		mov rdx, EqAhoMsgLen		; about new term
		syscall				;call system interrupt

		pop rax				;load index

		xor rdx, rdx			;clear rdx to be equal dl
		mov rcx, EqAhoBufLen - 1	;set max loop count
.prn_loop:
		mov dl, al			;dl = low byte
		and dl, 0x0f			; = low digit
		mov dl, [DbAhoHexMap + rdx]	; = symb[digit]
		mov BYTE [DbAhoBuf + rcx], dl	;-> buffer

		dec cx				;--cnt

		shr eax, 4			;num /= 16
		cmp eax, 0			; check for end
		jne .prn_loop			;proc next digit

		mov rax, 0x1			;system write
		mov rdi, 0x1                    ; to stdout
		lea rsi, [DbAhoBuf + rcx + 1]	;off = buf + cnt + 1
		mov rdx, EqAhoBufLen		;len = buf_len
		sub rdx, rcx                    ; -= cnt
		syscall				;call system interrupt

		pop rbx
		pop rbp

		ret				;return
;===============
;ENDP aho_print_term
;===============
