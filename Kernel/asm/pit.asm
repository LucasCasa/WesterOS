
GLOBAL set_PIT

set_PIT:

push rbp
mov rbp, rsp

mov al, 0x36
out 0x43, al    ;tell the PIT which channel we're setting

and rdi,0xFFFF
mov rax, rdi
out 0x40, al    ;send low byte
mov al,ah
out 0x40, al    ;send high byte

mov rsp,rbp
pop rbp

ret
