.global GDTFlush
.type GDTFlush, @function
GDTFlush:
	lgdt (%rdi)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	pushq $0x08   # push CS
	pushq 8(%rsp) # push return address
	retfq $0x08   # return there, clearing old address
