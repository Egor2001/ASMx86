.model tiny
.186
locals @@

;===============
;RING functions
;===============
extrn rb_size : proc
extrn rb_push : proc
extrn rb_pop : proc

;===============
;FILE functions
;===============
extrn fl_open : proc
extrn fl_close : proc
extrn fl_creat : proc
extrn fl_write : proc

public Main

.code 

;===============
;DEFINES
;===============
VIDEO_SEG       equ 0b800h                      ;video segment start
VIDEO_OFFSET    equ (5*80 + 40)*2               ;video offset
VIDEO_COLOR     equ 0efh                        ;video color
INT_KEYB        equ 09h                         ;keyboard interrupt number
;===============

;===============
;VARIABLES
;===============
sys_timer       dw 0                            ;system timer handler offset
                dw 0                            ;system timer handler segment

sys_keyb        dw 0                            ;system keyb handler offset
                dw 0                            ;system keyb handler segment

sys_idle        dw 0                            ;system idle handler offset
                dw 0                            ;system idle handler segment

RBsync          db 0
FLname          db '#log',0

cur_symb        db '@'                          ;last recognized symbol
;===============

;===============
;PROC           TIMER HANDLER
;===============
usr_timer       proc
                pushf
                call dword ptr cs:[cs:sys_timer];handler chaining
                push ax bx dx es
                
                mov ah, 34h                     ;check if DOS busy
                int 21h                         ; call DOS
                cmp word ptr es:[bx], 0         ; check busy and error flags

                jne @@EndOfInt                  ;skip if busy

                push cx                         ;set data segment
                pop dx                          ; to PSR program segment

                call symb_printer

@@EndOfInt:
                mov al, 20h                     ;end of interrupt
                out 20h, al                     ; signal

                pop es dx bx ax
                iret
usr_timer       endp
;===============
;ENDP           TIMER HANDLER
;===============

;===============
;PROC           KEYBOARD HANDLER
;===============
usr_keyb        proc
                push ax bx dx es
                
                push cx                         ;set data segment
                pop dx                          ; to PSR program segment

                in al, 60h                      ;read key scan-code

                mov ah, al                      ;check if key released
                and ah, 80h                     ; via checking 
                jnz @@EndOfInt                  ;skip if key released

                mov dl, al                      ;save scan-code in DL

                mov ax, 40h                     ;store BIOS queue segment
                mov es, ax                      ; in ES
                mov bx, 1eh                     ; store BIOS queue offset
                mov ah, byte ptr es:[bx]        ; load symbol from DOS queue
                mov cs:cur_symb, ah             ; store symbol in memory
                
                                                ;########
                jmp @@TempSkip                  ;skip incorrect code

@@SyncPush:                                     ;no idea
                cmp cs:RBsync, 0                ; why it doesn't work
                jne @@SyncPush                  ; (naive sync)

                not cs:RBsync                             
                call rb_push                    ;hangs up program
                not cs:RBsync                   ; I have no idea why

@@TempSkip:                                     ;temp label to skip prev code
                                                ;########

                call symb_handler               ;handle current symbol

@@EndOfInt:
                mov al, dl                      ;restore scan-code from DL
                in al, 61h                      ;some handling
                mov ah, al                      ; we need to
                or al, 80h                      ; pass symbol
                out 61h, al                     ; to control port
                xchg ah, al                     ; with different
                out 61h, al                     ; "enable kbd" bit

                pop es dx bx ax
                jmp dword ptr cs:[cs:sys_keyb]  ;handler chaining

                iret
usr_keyb        endp
;===============
;ENDP           KEYBOARD HANDLER
;===============

usr_idle        proc
                push ax bx cx dx es
                
                push cx                         ;set data segment
                pop dx                          ; to PSR program segment
                
                mov al, cs:cur_symb
                jz @@EndOfInt

                call symb_printer

                mov al, 0
                mov cs:cur_symb, al

@@EndOfInt:
                pop es dx cx bx ax
                jmp dword ptr cs:[cs:sys_idle]
                iret
usr_idle        endp

;===============
;PROC           symb_handler
;===============
;LAYOUT:
;AFFECT:
;       AX, BX, ES
;===============
symb_handler    proc
                mov ax, VIDEO_SEG               ;load video segment
                mov es, ax                      ; in ES
                mov ah, VIDEO_COLOR             ; set color
                mov bx, VIDEO_OFFSET            ; set offset

                mov al, cs:cur_symb             ;load current symbol

                                                ;##########
                jmp @@TempSkip                  ;skip incorrect code

@@SyncPop:                                      ;sync loop        
                cmp cs:RBsync, 0                ;check if not busy
                jne @@SyncPop                   ;loop until free

                not cs:RBsync                   ;enter critical section 
                call rb_pop                     ;pop from ring buffer
                not cs:RBsync                   ;leave critical section

@@TempSkip:                                     ;temp label to skip prev code
                                                ;##########

                mov word ptr es:[bx], ax        ;put symbol to console

                ret
symb_handler    endp
;===============
;ENDP           symb_handler
;===============

;===============
;PROC           symb_printer
;===============
;LAYOUT:
;AFFECT:
;       AX, BX, CX, DX
;===============
symb_printer    proc

                mov ax, VIDEO_SEG+1             ;load video segment
                mov es, ax                      ; in ES
                mov ah, VIDEO_COLOR             ; set color
                mov al, cs:cur_symb

                mov bx, VIDEO_OFFSET            ; set offset
                mov word ptr es:[bx], ax        ;put symbol to console

                push cx
                pop dx

                mov dx, offset cs:FLname        ;set filename
                call fl_open                    ;open file

                mov dx, offset cs:cur_symb      ;set data address 
                mov cx, 1                       ;set data length
                call fl_write                   ;write to file

                call fl_close                   ;close file

                ret
symb_printer    endp
;===============
;ENDP           symb_printer
;===============

;===============
;ENTRY POINT
;===============
Main:
                mov dx, offset FLname           ;set filename
                call fl_creat                   ;create file
                call fl_close

                mov ax, 08h                     ;load new timer handler
                mov bx, offset usr_timer
                call set_handler

                mov ax, es                      ;store old timer handler
                mov sys_timer+2, ax
                mov ax, bx
                mov sys_timer, ax

                mov ax, 09h                     ;load new keyboard handler
                mov bx, offset usr_keyb
                call set_handler

                mov ax, es                      ;store old keyboard handler
                mov sys_keyb+2, ax
                mov ax, bx
                mov sys_keyb, ax

                mov ax, 28h                     ;load new dos_idle handler
                mov bx, offset usr_idle
                call set_handler

                mov ax, es                      ;store old dos_idle handler
                mov sys_idle+2, ax
                mov ax, bx
                mov sys_idle, ax

@@EndOfSetup:
                mov ax, 3100h                   ;move to psr state
                mov dx, offset Main             ; reserve memory for handlers
                shr dx, 4                       ; store paragraph count in DX
                inc dx                          ; size = setup+1
                int 21h
;===============
;HLT
;===============

;===============
;PROC           set_handler
;===============
;set interrupt handler (analogue to DOS INT 21H 25H && DOS INT 21H 35H)
;Layout:
;       AX - interrupt number
;       CS:BX - new handler address
;Return:
;       ES:BX - old handler address
;Affect:
;       AX BX DX ES
;===============
set_handler     proc
                mov dx, 0                       ;load segment of
                mov es, dx                      ; interrupt vector in ES

                shl ax, 2                       ;calc handler offset

                mov dx, bx                      ;DX new
                mov bx, ax                      ;BX idx

                cld
                mov di, bx                      ;DI idx DX new
                mov ax, word ptr es:[bx+2]      ;AX seg
                mov bx, word ptr es:[bx]        ;BX old

                cli                             ;interrupt lock
                xchg ax, dx                     ;AX new BX old DX seg
                stosw                           ;set int09h offset
                mov ax, cs                      ;BX old DX seg
                stosw                           ;set new segment
                sti                             ;interrupt unlock

                mov es, dx                      ;store old segment in ES

                ret
set_handler     endp
;===============
;ENDP           set_handler
;===============

end
