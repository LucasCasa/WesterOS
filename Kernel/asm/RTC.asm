GLOBAL _set_value
GLOBAL _get_value
    
_get_value:	    ; lee del RTC
        cli
        push rbp
        mov rbp,rsp

        xor eax,eax
        mov rax,rdi

        out 0x70,al
        in  al,71h
        mov rsp,rbp
        pop rbp
        sti
	and rax, 0xFF
        ret

    
_set_value:		;escribe en el RTC
        cli             ;clear interrupts
        push rbp
        mov rbp,rsp
        mov rax,rdi
        out 0x70,al
        mov rax,rsi
        out 0x71,al     
	
	mov rsp,rbp
	pop rbp
        sti
        ret 

