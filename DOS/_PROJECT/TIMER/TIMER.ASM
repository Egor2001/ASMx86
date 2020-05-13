locals @@
.model tiny
.186

.code
org 100h

OldTimer        dw 0, 0

cnt             db 1
msg             db 'message', 10, 13, '$'

UsrTimer:
                call dword ptr cs:[OldTimer]
                pusha

                mov cl, cnt
                mov ch, cnt
                mov ax, 0b800h
                mov es, ax
                mov dx, 0
                mov bx, msg
                call strncpy

                mov ax, 1
                cmp ax, cnt

                jne UsrTimerRet

                mov cnt, 0ffh
                
UsrTimerRet:
                popa
                iret

Start:
                xor ax, ax
                mov es, ax
                mov bx, 08h*4

                mov ax, word ptr es:[bx]
                mov OldTimer, ax
                mov ax, word ptr es:[bx+2]
                mov OldTimer+2, ax

                cli
                mov ax, cs
                mov word ptr es:[bx+2], ax
                mov word ptr es:[bx], offset UsrTimer
                sti

                mov dx, offset Start
                shr dx, 4
                inc dx
                mov ax, 3100h
                int 21h

strcpy          proc
                mov di, bx
                mov cx, 0ffffh
                repne movsb

                ret
strcpy          endp

end             Start
