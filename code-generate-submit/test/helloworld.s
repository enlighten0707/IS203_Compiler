# start of generated code
	.text	
main:
	.globl	main
	.type	main, @function
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	xorq	%r15, %r15
	pushq	 %r15
	pushq	 %rdi
	pushq	 %rsi
	pushq	 %rdx
	pushq	 %rcx
	pushq	 %r8
	pushq	 %r9
	subq	$8, %rsp
	movsd	%xmm0, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm1, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm2, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm3, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm4, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm5, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm6, 0(%rsp)
	subq	$8, %rsp
	movsd	%xmm7, 0(%rsp)
	movq	$.LC0, %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	 printf
	movsd	0(%rsp), %xmm7
	addq	$8, %rsp
	movsd	0(%rsp), %xmm6
	addq	$8, %rsp
	movsd	0(%rsp), %xmm5
	addq	$8, %rsp
	movsd	0(%rsp), %xmm4
	addq	$8, %rsp
	movsd	0(%rsp), %xmm3
	addq	$8, %rsp
	movsd	0(%rsp), %xmm2
	addq	$8, %rsp
	movsd	0(%rsp), %xmm1
	addq	$8, %rsp
	movsd	0(%rsp), %xmm0
	addq	$8, %rsp
	popq	 %r9
	popq	 %r8
	popq	 %rcx
	popq	 %rdx
	popq	 %rsi
	popq	 %rdi
	movq	$1, %rax
	popq	 %r15
	addq	%r15, %rsp
	popq	 %r15
	popq	 %r14
	popq	 %r13
	popq	 %r12
	popq	 %rbx
	popq	 %rbp
	ret	
	popq	 %r15
	addq	%r15, %rsp
	.size	main, .-main
	.section	.rodata
.LC0:
	.string	"Hello world!"

# end of generated code
