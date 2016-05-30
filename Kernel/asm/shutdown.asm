GLOBAL _shutdown_asm

_shutdown_asm:
		cli
		push    rbp
    	mov     rbp, rsp
		mov al,0xFE
		out 64h,al
		leave
		ret