GLOBAL _call_int80

_call_int80:
    push    rbp
    mov     rbp, rsp
    int     80h
    leave
    ret