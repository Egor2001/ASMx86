.model tiny

.data
VIDEO_SEG       equ 0b800h

src_str         db 'str1',10,13,'$'
dest_str        db 'str2',10,13,'$'

WND_WIDTH       equ 80
WND_HEIGHT      equ 25
SYMB            equ 'a'

COORD_X         equ 40
COORD_Y         equ 5
BOX_WIDTH       equ 10
BOX_HEIGHT      equ 10

extrn wmemset : proc
extrn memcpy : proc

.code 
org 100h

Start:          
                mov ax, VIDEO_SEG
                mov es, ax
                mov al, SYMB
                mov ah, 0fh
                mov cx, WND_WIDTH
                mov di, 0
                cld
                call wmemset

                mov ax, VIDEO_SEG
                mov es, ax
                mov ax, (WND_WIDTH*COORD_Y + COORD_X)*2
                mov di, ax
                mov dl, BOX_WIDTH
                mov dh, BOX_HEIGHT
                call draw_box

                mov ax, ds
                mov es, ax
                mov cx, 4
                mov si, offset src_str
                mov di, offset dest_str
                cld
                call memcpy

                mov ah, 00h
                int 21h

;===============
;draw_line - draws line for a box
;LAYOUT:
;       ES - segment
;       DI - index
;       AX - mid word
;       CX - length
;       DL - beg char
;       DH - end char
;AFFECT:
;       CX, DI
;===============
draw_line       proc
                mov byte ptr es:[di], dl
                mov byte ptr es:[di+1], ah

                add di, 2
                call wmemset

                mov byte ptr es:[di], dh
                mov byte ptr es:[di+1], ah

                ret
draw_line       endp

;===============
;draw_box - draws (!)... a box(!!)
;LAYOUT:
;       ES - segment
;       DI - index
;       DL - dx
;       DH - dy
;AFFECT:
;       AX, BX, CX, DX, DI
;===============
BOX_VERT        equ '-'
BOX_HORZ        equ '|'
BOX_LTUP        equ '+'
BOX_RTUP        equ '+'
BOX_LTDN        equ '+'
BOX_RTDN        equ '+'
BOX_SYMB        equ ' '
BOX_COLOR       equ 0eh

draw_box        proc

                mov ax, 0
                mov cx, ax
                mov cl, dl

                mov bx, dx

                mov al, BOX_VERT
                mov ah, BOX_COLOR
                mov dl, BOX_LTUP
                mov dh, BOX_RTUP
                call draw_line

                mov dx, bx

                mov ax, 0
                mov cx, ax
                sub dh, 2
                mov cl, dh

                add di, (WND_WIDTH - 1)*2

                mov ax, 0
                mov al, dl
                shl al, 1

                sub di, ax
DrawLoop:
                push cx

                mov ax, 0
                mov cx, ax
                mov cl, dl

                mov bx, dx

                mov al, BOX_SYMB
                mov ah, BOX_COLOR
                mov dl, BOX_HORZ
                mov dh, BOX_HORZ
                call draw_line

                mov dx, bx

                pop cx
                add di, (WND_WIDTH - 1)*2

                mov ax, 0
                mov al, dl
                shl al, 1

                sub di, ax

                loop DrawLoop

                mov ax, 0
                mov cx, ax
                mov cl, dl

                mov bx, dx

                mov al, BOX_VERT
                mov ah, BOX_COLOR
                mov dl, BOX_LTDN
                mov dh, BOX_RTDN
                call draw_line

                mov dx, bx

                ret
draw_box        endp

end             Start

