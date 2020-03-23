BITS 64

global _start

SECTION .rodata
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

DbAhoMsg:	db 'detected term: ',0xA,0
EqAhoMsgLen:	equ ($ - DbAhoMsg)

DbAhoHexMap:	db '0123456789abcdef'

SECTION .data
DbAhoBuf:	db 0,0,0,0,0,0,0,0
EqAhoBufLen:	equ ($ - DbAhoBuf)

DbSrcStr:	db 'abacababca',0

SECTION .text
;===============
;PROC main
;===============
_start:
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
		mov rdi, rax			;str_addr

		mov rcx, 0
		mov rsi, 0			;state_idx = 0
.parse_loop:
		xor rax, rax			;cleanup before calc idx
		mov al, [rdi]			; = symb
		sub al, 'a'
		mul DWORD [DdDfaSize]		; *= DFA_SIZE
		add rax, rsi			; += state_idx
		shl rax, 2			; *= sizeof(DWORD)

		mov esi, [DdDfaEdgeMap + rax]	;state_idx = edge_map[rdx]
		mov ebx, esi
.search_loop:
		cmp DWORD [DdDfaTermMap + ebx*4], 0
		je .search_end

		push rdi
		push rsi
		push rcx
		mov rax, rcx
		call aho_print_term
		pop rcx
		pop rsi
		pop rdi

		mov ebx, [DdDfaLinkArr + ebx*4]	;
		cmp ebx, 0
		jne .search_loop		;proc next link

.search_end:
		inc rcx
		inc rdi
		cmp BYTE [rdi], 0
		jne .parse_loop			;proc next state

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
		lea rsi, [DbAhoMsg + rcx + 1]   ;off = buf + cnt + 1
		mov rdx, EqAhoMsgLen            ;len = buf_len
		sub rdx, rcx                    ; -= cnt
		syscall				;call system interrupt

		ret				;return
;===============
;ENDP aho_print_term
;===============
