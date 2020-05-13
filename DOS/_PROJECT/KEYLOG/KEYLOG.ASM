.model tiny

.data

.code
org 100h

Start:
                mov ax, 0b800h
                mov es, ax
                mov bx, 0

PollLoop:       
                xor ah, ah
                int 16h
                mov dl, al
                mov dh, 0fh

                mov word ptr es:[bx], dx

                cmp dl, 'q'
                jne  PollLoop

                mov ah, 00h
                int 21h
end             Start
