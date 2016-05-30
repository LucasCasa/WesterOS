

GLOBAL lights


lights:
	cli
	push rbp
	mov rbp, rsp
	mov rax, 0xED
	out 60h,al
	
ciclo:
	in 	al,60h
	cmp al, 0xFA
	jne	ciclo
;esperar:
;	in al,64h
;	cmp al,2
;	jne esperar
send:
	mov rax, rdi
	out 60h,al
	sti
	leave
	ret

;------------------------------;
;  Update the keyboard LED?s   ;
;------------------------------;
update_leds:
     push 	rbp
	 mov 	rbp, rsp
	 mov 	rbx,rdi
     call     kbd_wait

     mov     al, 0xED
     out     0x60, al

     call     kbd_wait

     mov     al, bl
     out     0x60, al

     call     kbd_wait

     leave
     ret



;------------------;
;  keyboard wait   ;
;------------------;
kbd_wait:
     jmp     $+2
     in     al,64h
     test     al,1
     jz     .ok
     jmp     $+2
     in     al,60h
     jmp     kbd_wait
     .ok:
     test     al,2
     jnz     kbd_wait
     ret

