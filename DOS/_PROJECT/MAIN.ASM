.model tiny
.186

.code 
org 100h 

;EXTERN RING BUFFER
;===============
extrn RB : byte
extrn RBidx : byte
extrn RBsize : byte

extrn rb_push : proc
extrn rb_pop : proc
;===============

;EXTERN KEYLOGGER
;===============
extrn KLsymb : byte
extrn KLcode : byte

extrn kl_ascii : proc
extrn kl_empty : proc
;===============

;EXTERN CSTRING
;===============
extrn memset : proc
extrn wmemset : proc

extrn memcpy : proc
extrn wmemcpy : proc

extrn memcmp : proc
extrn wmemcmp : proc
;===============

;EXTERN INTERRUPT
;===============
extrn int_set_handler : proc
extrn int_get_handler : proc
;===============

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
                push ax
                ;pushf
                
                mov al, 20h                             ;end of interrupt
                out 20h, al                             ; signal

                ;popf
                pop ax
                jmp dword ptr cs:[sys_timer]            ;handler chaining
                iret
;===============

;KEYBOARD HANDLER
;===============
usr_keyb:
                pushf
                push ax bx es

                in al, 60h                              ;read key scan-code

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
                call int_set_handler

                mov ax, es                              ;store old keyb
                mov sys_keyb+2, ax 
                mov sys_keyb, bx

                mov ax, 3100h
                mov dx, offset Setup                    ;reserve memory
                shr dx, 4                               ; size = setup+1
                inc dx                                  ; for psr handlers
                int 21h

HKeyboard:
                iret

HTimer:
                iret

HIdle:
                iret

draw_box        proc
draw_box        endp

draw_log        proc
draw_log        endp

end             Start
