
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _write_port

GLOBAL _int_timer_hand
GLOBAL _int_keyboard_hand
GLOBAL _int_piano_hand
GLOBAL _int80_hand
GLOBAL _int_start_sound
GLOBAL _int_end_sound
GLOBAL _beep
GLOBAL _song_note
GLOBAL _start_userland
GLOBAL _inactive_process

GLOBAL haltcpu
GLOBAL _call_int80

EXTERN timer_handler
EXTERN keyboard_handler
EXTERN sys_manager
EXTERN piano_handler
EXTERN switch_user_to_kernel
EXTERN switch_kernel_to_user
EXTERN get_entry_point
EXTERN print_number

%macro	pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro	popState 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro


SECTION .text

_cli:
	cli
	ret


_sti:
	sti
	ret

_write_port:
    push rbp
    mov rbp, rsp
    mov edx, edi
    mov rax, rsi
    and rax, 0FFFFh
    out dx, al
    mov rsp,rbp
    pop rbp
    ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, [rbp+8]
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, [rbp+8]  ; ax = mascara de 16 bits
    out		0A1h,al
    pop     rbp
    retn

_int80_hand:
    call sys_manager
    iretq

_int_timer_hand: ;Handler de INT 8 ( Timer tick)

		pushState; Se salvan los registros
		; save current process's RSP
		mov rdi, rsp
		mov rcx,rsp
		; enter kernel context by setting current process's kernel-RSP
		call switch_user_to_kernel
		mov rsp, rax

		;mov al,10h
		;mov es, ax
		;mov ds, ax

		call 		timer_handler
		; schedule, get new process's RSP and load it
		call switch_kernel_to_user
		;cmp rcx,rax
		;je eqls
		;mov rdi,rax
		;call print_number
eqls:
		mov rsp, rax

	; Send end of interrupt
    mov		al,20h			; Envio de EOI generico al PIC
	out		20h,al

		popState
    iretq

_start_userland:
	  	call switch_kernel_to_user
	  	mov rsp, rax

	  	call get_entry_point
	  	jmp rax

_int_keyboard_hand:				; Handler de INT 9 ( Teclado )
    pushState                      ; Se salvan los registros

    xor     rax,rax
	in 		al, 60h	              ;Leo el puerto del teclado
	and     rax,0x00000000000000FF
    xor     rdi,rdi
    mov     rdi,rax	            ;Le envio el SCAN CODE como parametro a la funcion int_09
	call 	keyboard_handler	;Llamo a la interrupcion que maneja el SCAN CODE en C


    mov		al,20h			; Envio de EOI generico al PIC
	out		20h,al

    popState
    iretq

_call_int80:
    push    rbp
    mov     rbp, rsp
    int     80h
    leave
    ret

haltcpu:
	cli
	hlt
	ret

_inactive_process:
	hlt
	ret

_int_start_sound:

        mov     al, 182         ; Prepare the speaker for the
        out     43h, al         ;  note.
		xor 	rax, rax
        mov     rax, rdi        ; Frequency number (in decimal)
        out     42h, al         ; Output low byte.
        mov     al, ah          ; Output high byte.
        out     42h, al
        in      al, 61h         ; Turn on note (get value from
                                ;  port 61h).
        or      al, 00000011b   ; Set bits 1 and 0.
        out     61h, al         ; Send new value.
        ; Send new value.
	ret

_beep:

        mov     al, 182         ; Prepare the speaker for the
        out     43h, al         ;  note.
        mov     rax, 4560        ; Frequency number (in decimal)
                                ;  for middle C.
        out     42h, al         ; Output low byte.
        mov     al, ah          ; Output high byte.
        out     42h, al
        in      al, 61h         ; Turn on note (get value from
                                ;  port 61h).
        or      al, 00000011b   ; Set bits 1 and 0.
        out     61h, al         ; Send new value.
        mov     bx, 320          ; Pause for duration of note.
.pause1:
        mov     cx, 65535
.pause2:
        dec     cx
        jne     .pause2
        dec     bx
        jne     .pause1
        in      al, 61h         ; Turn off note (get value from
                                ;  port 61h).
        and     al, 11111100b   ; Reset bits 1 and 0.
        out     61h, al         ; Send new value.
    ret

_song_note:

        mov     al, 182         ; Prepare the speaker for the
        out     43h, al         ;  note.
		xor		rax, rax
        mov     rax, rdi        ; Frequency number (in decimal)
        out     42h, al         ; Output low byte.
        mov     al, ah          ; Output high byte.
        out     42h, al
        in      al, 61h         ; Turn on note (get value from
	                                ;  port 61h).
        or      al, 00000011b   ; Set bits 1 and 0.
        out     61h, al         ; Send new value.
        mov     bx, si          ; Pause for duration of note.
	.pause1:
        mov     cx, 65535
	.pause2:
        dec     cx
        jne     .pause2
        dec     bx
        jne     .pause1
        in      al, 61h         ; Turn off note (get value from
	                                ;  port 61h).
        and     al, 11111100b   ; Reset bits 1 and 0.
        out     61h, al         ; Send new value.
	    ret


_int_end_sound:
 	in      al, 61h         ; Turn off note (get value from
 		                    ;  port 61h).
 	and al, -4
 	out 61h, al
 	ret
