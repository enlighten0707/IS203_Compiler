# start of generated code
	.text	
	.globl	main
	.type	main, @function
main:
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	subq	$8, %rsp
	subq	$8, %rsp
	subq	$8, %rsp
	subq	$8, %rsp
	movq	$0x400e666660000000, %rax
	movq	%rax, -8(%rsp)
	movsd	-8(%rsp), -48(%rbp)
	movq	$0x4012666660000000, %rax
	movq	%rax, -8(%rsp)
	movsd	-8(%rsp), -56(%rbp)
	movq	$1, %rax
	movq	%rax, -64(%rbp)
	movq	$10, %rax
	movq	%rax, -72(%rbp)
	movq	-72(%rbp), %rax
	cvtsi2sdq	%rax, %xmm8
	pushq	 %xmm8
	movq	-48(%rbp), %r12
	movq	%r12, -8(%rsp)
	movq	-8(%rsp), %xmm8
	popq	 %xmm9
	addsd	%xmm9, %xmm8
	movq	%rax, -8(%rsp)
	movsd	-8(%rsp), -56(%rbp)
	popq	 %r15
	popq	 %r14
	popq	 %r13
	popq	 %r12
	popq	 %rbx
	popq	 %rbp
	leave	
	ret	
	.size	main, .-main

# end of generated code
