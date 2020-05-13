.model tiny

.code                  

org 100h

Start:
                mov ax, 0b800h
                mov es, ax
                mov ax, 0000h

                mov cx, 0000h

                mov cx, 0000h
                mov cl, 60d

LoopX:
                mov ax, cx
                add ax, cx

                mov bx, ax
                mov word ptr es:[bx], 0fcdh

                mov ax, 0000h
                mov al, 10d
                mov dx, 2*80d
                mul dx
                add ax, cx
                add ax, cx

                mov bx, ax
                mov word ptr es:[bx], 0fcdh
                
                loop LoopX

                mov cx, 0000h
                mov cl, 10d
LoopY:
                mov ax, cx
                mov dx, 2*80d
                mul dx

                mov bx, ax
                mov word ptr es:[bx], 0fbah

                add ax, 2*60d

                mov bx, ax
                mov word ptr es:[bx], 0fbah
                
                loop LoopY

                int 21h

end             Start
