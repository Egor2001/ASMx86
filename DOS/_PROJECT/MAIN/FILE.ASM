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

                mov ax, 0b804h
                mov es, ax
                mov ah, 0eh
                mov al, byte ptr cs:handle
                mov word ptr es:[0], ax
                mov al, byte ptr cs:handle+1
                mov word ptr es:[2], ax

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
