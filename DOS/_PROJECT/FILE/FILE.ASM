.model tiny
.186

locals @@

.data
handle          dw 0
filename        db 'file', 0
data            db 0

.code
org 100h

Start:
                                        ;TODO: to check for errors
                mov dx, offset filename ;set file name
                mov cx, 0               ;normal file
                mov ah, 3ch             ;create file
                int 21h                 ;call DOS
                mov handle, ax          ;store handle

                mov data, 'A'
                call write_symb

                mov ax, 0000h           ;exit
                int 21h                 ;call DOS

write_symb      proc

                mov dx, offset data     ;buffer offset
                mov cx, 1               ;write 1 byte
                mov bx, handle          ;set file handle
                mov ah, 40h             ;write to file
                int 21h                 ;call DOS

                ret
write_symb      endp

end             Start
