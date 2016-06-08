GLOBAL _asm_set_cr3
GLOBAL _asm_get_cr3

_asm_set_cr3:
    push rbp
    mov rbp, rsp

    mov cr3, rdi

    mov rsp, rbp
    pop rbp
    ret


    _asm_get_cr3:
        push rbp
        mov rbp, rsp

        mov rax, cr3

        mov rsp, rbp
        pop rbp
        ret
