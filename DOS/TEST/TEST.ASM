.model tiny
.code
org 100h
start:		mov ah,02h
		mov dx,offset OutStr
		int 21h
		mov ax,4c00h
		int 21h

OutStr:		db 'duDos$',00h
end		start