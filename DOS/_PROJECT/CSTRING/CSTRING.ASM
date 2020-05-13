.model tiny

.code

public memset
public wmemset

public memcpy
public wmemcpy

public memcmp

public memchr
public memrchr

public strlen
public strcpy
public strcmp

public strchr
public strrchr

;===============
;memset - fill memory with specified byte
;Layout:
;       AX - word
;       CX - bytes count
;       flag DF - direction
;       DI - destination address
;       ES - destination segment
;Affects:
;       CX, DI 
;===============
memset          proc
                rep stosb

                ret
memset          endp

;===============
;wmemset - fill memory with specified word
;Layout:
;       AX - word
;       CX - words count
;       flag DF - direction
;       DI - destination address
;       ES - destination segment
;Affects:
;       CX, DI 
;===============
wmemset         proc
                rep stosw

                ret
wmemset         endp

;===============
;memcpy - copy memory byte - to byte 
;Layout:
;       flag DF - direction
;       CX - bytes count
;       SI - source address [DS is considered as a source segment]
;       DI - destination address
;       ES - destination segment
;Affects:
;       CX, SI, DI 
;===============
memcpy          proc
                rep movsb

                ret
memcpy          endp

;===============
;wmemcpy - copy memory word - to word
;Layout:
;       flag DF - direction
;       CX - words count
;       SI - source address [DS is considered as a source segment]
;       DI - destination address
;       ES - destination segment
;Affects:
;       CX, SI, DI 
;===============
wmemcpy         proc
                rep movsw

                ret
wmemcpy         endp

;===============
;memcmp - compare memory byte - to byte destination from source
;Layout:
;       flag DF - direction
;       CX - bytes count
;       SI - source address [DS is considered as a source segment]
;       DI - destination address
;       ES - destination segment
;Affects:
;       CX, SI, DI 
;===============
memcmp          proc
                repe cmpsb

                ret
memcmp          endp

;===============
;memrchr - find first character occurence in memory
;Layout:
;       AL - character
;       CX - length
;       DI - destination address
;       ES - destination segment
;Return:
;       CX - character position from start
;Affects:
;       AL, CX, DI 
;===============
memchr          proc

                cld
                rep scasb
                std

                ret
memchr          endp

;===============
;memrchr - find last character occurence in memory
;Layout:
;       AL - character
;       CX - length
;       DI - destination address
;       ES - destination segment
;Return:
;       CX - character position from end
;Affects:
;       AL, CX, DI 
;===============
memrchr         proc
                std
                rep scasb
                cld

                ret
memrchr         endp

;===============
;strlen - get '$'-terminated string length in bytes
;Layout:
;       flag DF - direction
;       DI - destination address
;       ES - destination segment
;Return:
;       CX - bytes count
;Affects:
;       AL, CX, DI 
;===============
strlen          proc
                mov cx, 0ffffh
                mov al, '$'
                repne scasb             ;TODO: when does it dec
                neg cx
                sub cx, 2

                ret
strlen          endp

;===============
;strcpy - copy string byte - to byte
;Layout:
;       flag DF - direction
;       SI - source address [DS is considered as a source segment]
;       DI - destination address
;       ES - destination segment
;Affects:
;       AL, CX, SI, DI 
;===============
strcpy          proc
                call strlen
                call memcpy

                ret
strcpy          endp

;===============
;strcmp - compare string byte - to byte; destination from source
;Layout:
;       flag DF - direction
;       SI - source address [DS is considered as a source segment]
;       DI - destination address
;       ES - destination segment
;Affects:
;       AL, CX, SI, DI 
;===============
strcmp          proc
                call strlen
                inc cx

                call memcmp

                ret
strcmp          endp

;===============
;strchr - find first character occurence in string
;Layout:
;       AL - character
;       flag DF - direction
;       DI - destination address
;       ES - destination segment
;Return:
;       CX - character position from start
;Affects:
;       AX, CX, SI, DI 
;===============
strchr          proc
                mov ah, al
                call strlen
                inc cx
                mov al, ah

                call memchr

                ret
strchr          endp

;===============
;strrchr - find last character occurence in string
;Layout:
;       AL - character
;       flag DF - direction
;       DI - destination address
;       ES - destination segment
;Return:
;       CX - character position from end
;Affects:
;       AX, CX, SI, DI 
;===============
strrchr         proc
                mov ah, al
                call strlen
                inc cx
                mov al, ah

                call memrchr

                ret
strrchr         endp

end
