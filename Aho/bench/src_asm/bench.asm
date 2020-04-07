;===============
;x86_64 configuration
;===============
BITS 64
;===============

;===============
;public function
;===============
global find_pattern

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
;PROC find_pattern
;	Parses input string 
;	and prints all pattern matches
;===============
;LAYOUT:
;	RDI: string address
;AFFECT:
;	RDI, RSI, RAX, RCX, RDX
;===============
find_pattern:
		sysv_prologue			;sysV64 convention

		mov rcx, 0
		mov rsi, 0			;state_idx = 0
.parse_loop:
		xor rax, rax			;cleanup before calc idx
		mov al, [rdi]			; = symb
		mul DWORD [rel DdDfaSize]	; *= DFA_SIZE
		add rax, rsi			; += state_idx
		shl rax, 2			; *= sizeof(DWORD)

		lea r8, [rel DdDfaEdgeMap]
		mov esi, [r8 + rax]		;state_idx = edge_map[rdx]
		mov ebx, esi
.search_loop:
		;rdx upper part clears automatically
		lea r8, [rel DdDfaTermMap]
		mov edx, [r8 + rbx*4]		;edx = is_term[ebx]
		cmp edx, 0			;check if term
		je .search_next			; and stop if not

		multipush rdi, rsi, rcx		;store affected registers

		mov rdi, rcx			;pass current position
		mov rsi, rdx			;pass matched string index
		dec rsi				;because of indexation from 1
		call detect_pattern		;print message

		multipop rdi, rsi, rcx		;load affected registers
.search_next:
		lea r8, [rel DdDfaLinkArr]
		mov ebx, [r8 + rbx*4]		;go to next link
		cmp ebx, 0			; and check for link to 0
		jne .search_loop		;proc next link or stop if 0

		inc rcx				;++cnt
		inc rdi				;++pos
		cmp BYTE [rdi], 0		; and check for '\0' symb
		jne .parse_loop			;proc next state or stop if 0

		sysv_epilogue			;sysV64 convention
		ret				;return
;===============
;ENDP find_pattern
;===============

;===============
;PROC detect_pattern
;	Prints end position of detected term
;===============
;LAYOUT:
;	RAX: state number
;AFFECT:
;	RDI, RSI, RAX, RCX, RDX
;===============
detect_pattern:
		sysv_prologue			;sysV64 convention

		mov rbx, rsi			;rbx = match string index
		multipush rdi			;store params

		mov rax, 0x1			;system write
		mov rdi, 0x1			; to stdout
		lea rsi, [rel DbAhoMsg]		;info str
		mov rdx, EqAhoMsgLen		; about new term
		syscall				;call system interrupt

		mov rax, 0x1			;system write
		mov rdi, 0x1			; to stdout
		lea r9, [rel DqAhoStrArr]
		mov rsi, [r9 + rbx*8]		;matched str
		lea r9, [rel DdAhoLenArr]
		mov edx, [r9 + rbx*4]		;matched str len
		syscall				;call system interrupt

		multipop rdi			;load params

		mov rax, rdi			;rax = match position
		xor rdx, rdx			;clear rdx to be equal dl
		mov rcx, EqAhoBufLen - 1	;set max loop count
.prn_loop:
		mov dl, al			;dl = low byte
		and dl, 0x0f			; = low digit
		lea r9, [rel DbAhoHexMap]
		mov dl, [r9 + rdx]		; = symb[digit]
		lea r9, [rel DbAhoBuf]
		mov BYTE [r9 + rcx], dl		;-> buffer

		dec rcx				;--cnt

		shr eax, 4			;num /= 16
		cmp eax, 0			; check for end
		jne .prn_loop			;proc next digit

		mov dx, '0x'			;dx = '0x'
		sub rcx, 2			;adjust start pos
		lea r9, [rel DbAhoBuf]
		mov [r9 + rcx + 1], dx		;add '0x' hex prefix

		mov rax, 0x1			;system write
		mov rdi, 0x1                    ; to stdout
		lea r9, [rel DbAhoBuf]
		lea rsi, [r9 + rcx + 1]		;off = buf + cnt + 1
		mov rdx, EqAhoBufLen		;len = buf_len
		sub rdx, rcx                    ; -= cnt
		syscall				;call system interrupt

		sysv_epilogue			;sysV64 convention
		ret				;return
;===============
;ENDP detect_pattern
;===============
