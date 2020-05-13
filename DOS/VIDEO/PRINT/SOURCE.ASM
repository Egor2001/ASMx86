.model tiny

.code                  

org 100h

Start:
                mov ax, 0b805h
                mov es, ax
                mov ax, 0000h

                mov bx, offset out_str

                mov cx, 00ffh
                mov ax, 0000h
LoopPrint:
                mov dl, byte ptr cs:[bx]
                cmp dl, 10
                je Return

                mov bx, ax
                add bx, ax
                mov dh, 0fh
                mov word ptr es:[bx], dx

                inc ax

                mov bx, offset out_str
                add bx, ax
                
                loop LoopPrint

Return:
                int 21h

out_str         db 'time to sleep!', 10, 13, '$'

end             Start
