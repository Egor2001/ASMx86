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
INT_TIMER       equ 08h                 ;timer interrupt number
INT_KEYB        equ 09h                 ;keyboard interrupt number
;===============

;VARIABLES
;===============
sys_timer       dw 0                    ;system timer handler offset
                dw 0                    ;system timer handler segment

sys_keyb        dw 0                    ;system keyb handler offset
                dw 0                    ;system keyb handler segment

cur_symb        db 0                    ;last recognized symbol
;===============

;TIMER HANDLER
;===============
usr_timer:
                pushf
                push ax
                
                mov al, 20h                             ;end of interrupt
                out 20h, al                             ; signal

                pop ax
                popf
                jmp dword ptr cs:[sys_timer]            ;handler chaining
                iret
;===============

;KEYBOARD HANDLER
;===============
usr_keyb:
                pushf
                push ax bx es

                mov ax, VIDEO_SEG                       ;set indexation
                mov es, ax                              ; to video memory
                in al, 40h                              ;get position
                shl al, 1                               ; from timer counter
                mov bl, al                              ; and mul by 2
                mov bh, 0                               ;set position

                in al, 60h                              ;read key scan-code
                mov ah, 0fh                             ; set console color

                mov cur_symb, al                        ;save scan-code
                mov word ptr es:[bx], ax                ;output

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


;###############
;Setup:                                                 ;does not work
;                mov al, 0
;                out 40h, al
;                out 43h, al

;                mov ax, INT_TIMER                       ;set timer
;                mov bx, offset usr_timer
;                call SetHandler

;                mov ax, es                              ;store old timer
;                mov sys_timer+2, ax 
;                mov sys_timer, bx
;###############

;ENTRY POINT
;===============
Setup:
                mov ax, 09h                             ;set keyb
                mov bx, offset usr_keyb
                call SetHandler

                mov ax, es                              ;store old keyb
                mov sys_keyb+2, ax 
                mov sys_keyb, bx

                mov ax, 3100h
                mov dx, offset Setup                    ;reserve memory
                add dx, 4                               ; for psr handlers
                shr dx, 4                               ; size = setup+1
                int 21h
;===============

;PROCEDURE SetHandler
;===============
;set interrupt handler (analogue to DOS INT 21H 25H)
;Layout:
;       AX - interrupt number
;       CS:BX - new handler address
;Return:
;       ES:BX - old handler address
;Affect:
;       AX BX DX ES
;===============
SetHandler      proc
                mov dx, 0
                mov es, dx

                shl ax, 2

                mov dx, bx                              ;DX new
                mov bx, ax                              ;BX idx

                cld
                mov di, bx                              ;DI idx DX new
                mov ax, word ptr es:[bx+2]              ;AX seg
                mov bx, word ptr es:[bx]                ;BX old

                cli                                     ;interrupt lock
                xchg ax, dx                             ;AX new BX old DX seg
                stosw                                   ;set int09h offset
                mov ax, cs                              ;BX old DX seg
                stosw                                   ;set int09h segment
                sti                                     ;interrupt unlock

                mov es, dx

                ret
SetHandler      endp
;===============

end             Start
