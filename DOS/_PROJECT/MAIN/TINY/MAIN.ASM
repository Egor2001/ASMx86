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

.code 
org 100h 

;===============
;ENTRY POINT
;===============
Start:          jmp Setup 
;===============

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
FLname          db '#log', 0

cur_symb        db '@'                          ;last recognized symbol
test_str        db 'ku!',10,13,'$'
;===============

;===============
;PROC           TIMER HANDLER
;===============
usr_timer       proc
                pushf
                call dword ptr cs:[sys_timer]   ;handler chaining
                push ax bx dx es

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
                jmp dword ptr cs:[sys_keyb]     ;handler chaining

                iret
usr_keyb        endp
;===============
;ENDP           KEYBOARD HANDLER
;===============

usr_idle        proc
                ;jmp dword ptr cs:[sys_idle]

                push ax bx cx dx es
                
                mov al, cs:cur_symb
                jz @@EndOfRet

                call symb_printer
                mov al, 0
                mov cs:cur_symb, al

@@EndOfRet:
                pop es dx cx bx ax
                jmp dword ptr cs:[sys_idle]
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

                mov dx, offset cs:FLname        ;set filename
                call cs:fl_open                 ;open file

                mov dx, offset cs:cur_symb      ;set data address 
                mov cx, 1                       ;set data length
                call cs:fl_write                ;write to file

                call cs:fl_close                ;close file

                ret
symb_printer    endp
;===============
;ENDP           symb_printer
;===============

;===============
;ENTRY POINT
;===============
Setup:
                mov 
                int 21h

@@EndOfSetup:
                mov ax, 3100h                   ;move to psr state
                mov dx, offset Setup            ; reserve memory for handlers
                shr dx, 4                       ; store paragraph count in DX
                inc dx                          ; size = setup+1
                int 21h
;===============
;HLT
;===============

end             Start
.model tiny
.186

locals @@

public fl_open
public fl_close
public fl_creat
public fl_write

.code

handle          dw 0
filename        db 'file', 0

;===============
;PROC           fl_open
;===============
;LAYOUT:
;       DS:DX - filename
;AFFECT:
;       AX, CX
;===============
fl_open         proc
                mov al, 1               ;write mode
                mov ah, 3dh             ;open file
                int 21h                 ;call DOS
                mov cs:handle, ax       ;store handle

                ret
fl_open         endp
;===============
;ENDP           open_file
;===============

;===============
;PROC           fl_close
;===============
;LAYOUT:
;AFFECT:
;       AX, BX
;===============
fl_close        proc
                mov ah, 3eh             ;close file
                mov bx, cs:handle       ;store handle
                int 21h                 ;call DOS

                ret
fl_close        endp
;===============
;ENDP           close_file
;===============

;===============
;PROC           fl_creat
;===============
;LAYOUT:
;       DS:DX - filename
;AFFECT:
;       AX, CX
;===============
fl_creat        proc
                mov cx, 0               ;normal file
                mov ah, 3ch             ;create file
                int 21h                 ;call DOS
                mov cs:handle, ax       ;store handle

                ret
fl_creat        endp
;===============
;ENDP           fl_creat
;===============

;===============
;PROC           fl_write
;===============
;LAYOUT:
;       DS:DX - source
;       CX - length
;AFFECT:
;       AX, BX
;===============
fl_write        proc
                mov bx, cs:handle       ;set file handle
                mov ah, 40h             ;write to file
                int 21h                 ;call DOS

                ret
fl_write        endp
;===============
;ENDP           fl_write
;===============

end
