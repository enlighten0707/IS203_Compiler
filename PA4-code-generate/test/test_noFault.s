# start of generated code
	.text	
main:
	.global	main
	.type	main, @function
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	xorq	%r15, %r15
	addq	$8, %r15
	subq	$8, %rsp
	pushq	 %r15
	movq	$0x400e666660000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -48(%rbp)
	pushq	 %rdi
	pushq	 %rsi
	pushq	 %rdx
	pushq	 %rcx
	pushq	 %r8
	pushq	 %r9
	subq	$8, %rsp
	movsd	%xmm0, (%rsp)
	subq	$8, %rsp
	movsd	%xmm1, (%rsp)
	subq	$8, %rsp
	movsd	%xmm2, (%rsp)
	subq	$8, %rsp
	movsd	%xmm3, (%rsp)
	subq	$8, %rsp
	movsd	%xmm4, (%rsp)
	subq	$8, %rsp
	movsd	%xmm5, (%rsp)
	subq	$8, %rsp
	movsd	%xmm6, (%rsp)
	subq	$8, %rsp
	movsd	%xmm7, (%rsp)
	movq	$.LC0, %rax
	movq	%rax, %rdi
	movq	$0x400e666660000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movq	%xmm8, %xmm0
	subq	$8, %rsp
	movl	$1, %eax
	call	 printf
	addq	$8, %rsp
	movsd	(%rsp), %xmm7
	addq	$8, %rsp
	movsd	(%rsp), %xmm6
	addq	$8, %rsp
	movsd	(%rsp), %xmm5
	addq	$8, %rsp
	movsd	(%rsp), %xmm4
	addq	$8, %rsp
	movsd	(%rsp), %xmm3
	addq	$8, %rsp
	movsd	(%rsp), %xmm2
	addq	$8, %rsp
	movsd	(%rsp), %xmm1
	addq	$8, %rsp
	movsd	(%rsp), %xmm0
	addq	$8, %rsp
	popq	 %r9
	popq	 %r8
	popq	 %rcx
	popq	 %rdx
	popq	 %rsi
	popq	 %rdi
	popq	 %r15
	addq	%r15, %rsp
	popq	 %r15
	popq	 %r14
	popq	 %r13
	popq	 %r12
	popq	 %rbx
	popq	 %rbp
	ret	
	.size	main, .-main
	.section	.rodata
.LC0:
	.string	"x1 = %f
"

# end of generated code
