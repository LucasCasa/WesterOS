
GLOBAL outb
GLOBAL inb

outb:
	mov rdx, rsi
	mov rax, rdi
	out dx, al
	ret

inb:
	mov rax, 0
	mov rdx, rdi
	in al, dx
	mov rax, rdi
	ret