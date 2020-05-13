.model tiny

public rb_size
public rb_push
public rb_pop

.code

RB              db 100h dup(0)          ; ring buffer
RBidx           db 0ffh                 ; ring buffer first index
RBsize          dw 0                    ; ring buffer size (word)

;===================
;rb_size get size of ring buffer (may be more than 256)
;Layout:
;       AX - input value
;Affects:
;       AX 
;===================
rb_size         proc
                mov ax, RBsize
                ret
rb_size         endp

;===================
;rb_push push value to ring buffer
;Layout:
;       AL - input value
;Affects:
;       AX, BX, DX 
;===================
rb_push         proc
                mov dl, al

                inc RBidx
                mov al, RBidx
                mov ah, 0
                add ax, offset RB
                mov bx, ax

                mov byte ptr cs:[bx], dl
                inc RBsize

                ret
rb_push         endp

;===================
;rb_pop pop value from ring buffer
;Layout:
;       AL - return value
;Affects:
;       AX, BX
;===================
rb_pop          proc
                mov al, RBidx
                mov ah, 0
                add ax, offset RB
                mov bx, ax

                mov al, byte ptr cs:[bx]
                dec RBidx
                dec RBsize

                ret
rb_pop          endp

end 

