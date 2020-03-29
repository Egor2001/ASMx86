;===============
;x86_64 configuration
;===============
BITS 64
;===============

;===============
;entry point
;===============
global _start
;===============

;===============
;read-only data
;===============
SECTION .rodata

%include "macro.asm"				;include macros
%include "aho_dfa.asm"				;include generated dfa

DbAhoMsg:	db 'detected term: ',0xA,0	;message to print on match
EqAhoMsgLen:	equ ($ - DbAhoMsg)		;message len as offset diff

DbAhoHexMap:	db '0123456789ABCDEF'		;byte - to hex convertion map
;===============

;===============
;stack uninitialized data
;===============
SECTION .bss
EqInputMax	equ 0x1000			;input buffer length
DbInputBuf	resb EqInputMax			;input buffer
;===============

;===============
;read/write data
;===============
SECTION .data
DbAhoBuf:	db 0,0,0,0,0,0,0,0,0,0,0x0A	;buffer to store hex string
EqAhoBufLen:	equ ($ - DbAhoBuf) - 1		;buffer length
;===============

;===============
;program code
;===============
SECTION .text
;===============
;PROC main
;===============
_start:
		;no, I won't optimize non-critical parts with xor!
		mov rax, 0x0			;system read
		mov rdi, 0x0			; from stdin
		lea rsi, [DbInputBuf]		;to buffer
		mov rdx, EqInputMax - 1		; max len = 4095
		syscall				;call system interrupt

		lea rdi, [DbInputBuf]		;pass input buffer
		call aho_parse_str		;call parsing procedure

		mov rax, 0x3c			;exit
		mov rdi, 0			; with code 0
		syscall				;call system interrupt

		ret				;terminal canary
;===============
;ENDP main
;===============

;===============
;PROC aho_parse_str
;	Parses input string 
;	and prints all pattern matches
;===============
;LAYOUT:
;	RDI: string address
;AFFECT:
;	RDI, RSI, RAX, RCX, RDX
;===============
aho_parse_str:
		sysv_prologue			;sysV64 convention

		mov rcx, 0
		mov rsi, 0			;state_idx = 0
.parse_loop:
		xor rax, rax			;cleanup before calc idx
		mov al, [rdi]			; = symb
		mul DWORD [DdDfaSize]		; *= DFA_SIZE
		add rax, rsi			; += state_idx
		shl rax, 2			; *= sizeof(DWORD)

		mov esi, [DdDfaEdgeMap + rax]	;state_idx = edge_map[rdx]
		mov ebx, esi
.search_loop:
		;rdx upper part clears automatically
		mov edx, [DdDfaTermMap + ebx*4]	;edx = is_term[ebx]
		cmp edx, 0			;check if term
		je .search_next			; and stop if not

		multipush rdi, rsi, rcx		;store affected registers

		mov rdi, rcx			;pass current position
		mov rsi, rdx			;pass matched string index
		dec rsi				;because of indexation from 1
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
		sysv_prologue			;sysV64 convention

		mov rbx, rsi			;rbx = match string index
		multipush rdi			;store params

		mov rax, 0x1			;system write
		mov rdi, 0x1			; to stdout
		lea rsi, [DbAhoMsg]		;info str
		mov rdx, EqAhoMsgLen		; about new term
		syscall				;call system interrupt

		mov rax, 0x1			;system write
		mov rdi, 0x1			; to stdout
		mov rsi, [DqAhoStrArr + rbx*8]	;matched str
		mov edx, [DdAhoLenArr + rbx*4]	;matched str len
		syscall				;call system interrupt

		multipop rdi			;load params

		mov rax, rdi			;rax = match position
		xor rdx, rdx			;clear rdx to be equal dl
		mov rcx, EqAhoBufLen - 1	;set max loop count
.prn_loop:
		mov dl, al			;dl = low byte
		and dl, 0x0f			; = low digit
		mov dl, [DbAhoHexMap + rdx]	; = symb[digit]
		mov BYTE [DbAhoBuf + rcx], dl	;-> buffer

		dec rcx				;--cnt

		shr eax, 4			;num /= 16
		cmp eax, 0			; check for end
		jne .prn_loop			;proc next digit

		mov dx, '0x'			;dx = '0x'
		sub rcx, 2			;adjust start pos
		mov [DbAhoBuf + rcx + 1], dx	;add '0x' hex prefix

		mov rax, 0x1			;system write
		mov rdi, 0x1                    ; to stdout
		lea rsi, [DbAhoBuf + rcx + 1]	;off = buf + cnt + 1
		mov rdx, EqAhoBufLen		;len = buf_len
		sub rdx, rcx                    ; -= cnt
		syscall				;call system interrupt

		sysv_epilogue			;sysV64 convention
		ret				;return
;===============
;ENDP aho_print_term
;===============
