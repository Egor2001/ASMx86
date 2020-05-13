.model tiny

.code                  

org 100h

start:
                mov bx, offset string   ; pass string addr as arg
                mov ax, 0003h           ; video mode 3
                int 10h                 ; set video mode

                mov cx, 00ffh           ; initialize counter
                mov dx, 0000h           ;
loop1:                                  ; output loop
                mov bx, offset string
                mov ax, 00ffh
                sub ax, cx
                add bx, ax

                cmp byte ptr cs:[bx], 10   ; check for end of string
                je return               ; return

                mov dh, 13d             ; pass row arg
                mov dl, al              ; shift by counter
                mov ah, 02h             ; set cursor position
                mov bh, 00h             ; video page number
                int 10h                 ; bios video

                mov bx, offset string
                mov ax, 00ffh
                sub ax, cx
                add bx, ax

                push cx
                mov al, byte ptr cs:[bx]   ; pass symb arg
                mov ah, 0ah             ; 0ah due to bios docs
                mov bh, 00h             ; video page number
                mov cx, 01h             ; repeat count
                int 10h                 ; bios video
                pop cx

                mov ch, 00h

                inc dl                  ; ++ counter
                                        
                loop loop1              ; new loop
return:
                int 21h                 ; hlt

string          db 'fuck', 10, 13, '$'  ; crlf (?)

end             start
