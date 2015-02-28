
.text

######
# load a new GDT
#  parameter 1: address of gdtr
#  parameter 2: new code descriptor offset
#  parameter 3: new data descriptor offset
.global _x86_64_asm_lidt
_x86_64_asm_lidt:
	lidt (%rdi)
#	pushq %rsi                  # push code selector
#	movabsq $.done, %r10
#	pushq %r10                  # push return address
#	lretq                       # far-return to new cs descriptor ( the retq below )
#.done:
#	movq %rdx, %es
#	movq %rdx, %fs
#	movq %rdx, %gs
#	movq %rdx, %ds
#	movq %rdx, %ss
	retq
