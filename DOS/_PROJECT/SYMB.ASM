.model tiny
.186

.code 
org 100h 

;ENTRY POINT
;===============
Start:          jmp Setup 
;===============

;DEFINES
;===============
VIDEO_SEG       equ 0b800h              ;video segment start
INT_KEYB        equ 09h                 ;keyboard interrupt number
;===============

;VARIABLES
;===============
sys_keyb        dw 0                    ;system keyb handler offset
                dw 0                    ;system keyb handler segment

cur_symb        db 0                    ;last recognized symbol
;===============

;KEYBOARD HANDLER
;===============
usr_keyb:
                pushf
                push ax bx es

                in al, 60h                              ;read key scan-code

                mov cur_symb, al                        ;save scan-code
                call SymbHandler

                in al, 61h                              ;some handling
                mov ah, al                              ; we need to
                or al, 80h                              ; pass symbol
                out 61h, al                             ; to control port
                xchg ah, al                             ; with different
                out 61h, al                             ; "enable kbd" bit

                mov al, 20h                             ;end of interrupt
                out 20h, al                             ; signal

                pop es bx ax
                popf

                jmp dword ptr cs:[sys_keyb]             ;handler chaining
                iret
;===============

SymbHandler     proc
                mov ax, VIDEO_SEG
                mov es, ax
                mov ah, 0efh
                mov bx, (5*80 + 40)*2
                mov al, cur_symb
                mov word ptr es:[bx], ax                ;output

                ret
SymbHandler     endp

;ENTRY POINT
;===============
Setup:
                mov ax, 0
                mov es, ax

                mov bx, 09h*4
                mov ax, word ptr es:[bx+2]
                mov sys_keyb+2, ax
                mov ax, word ptr es:[bx]
                mov sys_keyb, ax

                mov ax, cs
                mov word ptr es:[bx+2], ax
                mov word ptr es:[bx], offset usr_keyb

                mov ax, VIDEO_SEG
                mov es, ax
                mov ah, 0efh
                mov bx, (5*80 + 40)*2

                mov ax, 3100h
                mov dx, offset Setup                    ;reserve memory
                shr dx, 4                               ; size = setup+1
                inc dx                                  ; for psr handlers
                int 21h

end             Start
