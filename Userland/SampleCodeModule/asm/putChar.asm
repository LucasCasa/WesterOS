

GLOBAL _put_char
GLOBAL _delete_char
GLOBAL _get_screen_char
GLOBAL _put_modifier


_put_char:
	push rbp
    mov rbp, rsp

    mov rax,rdi
    mov rbx,rsi
    mov rdi, 1
    mov rsi, rax
    mov rdx,rbx
    int 80h

    leave 
    ret
    
_put_modifier:
    push rbp
    mov rbp, rsp    

    mov rax,rdi
    mov rdi,4
    mov rsi,rax
    int 80h

    leave
    ret    

_delete_char:
    push rbp
    mov rbp, rsp

    mov rdi, 2
    int 80h

    leave 
    ret

_get_screen_char:
    push rbp
    mov rbp, rsp

    mov rdi,3
    int 80h

    leave
    ret
