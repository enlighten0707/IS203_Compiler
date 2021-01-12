# start of generated code
	.text	
tan:
	.global	tan
	.type	tan, @function
	pushq	 %rbp
	movq	%rsp, %rbp
	pushq	 %rbx
	pushq	 %r12
	pushq	 %r13
	pushq	 %r14
	pushq	 %r15
	xorq	%r15, %r15
	pushq	 %r15
	movsd	%xmm0, -8(%rsp)
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %r12
	pushq	 %r12
	movsd	%xmm2, -8(%rsp)
	movq	-8(%rsp), %xmm8
	popq	 %r13
	xorpd	%xmm9, %xmm9
	movq	%r13, %xmm9
	subsd	%xmm9, %xmm8
	movq	%xmm8, %r12
	pushq	 %r12
	movsd	%xmm1, -8(%rsp)
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %r12
	pushq	 %r12
	movsd	%xmm3, -8(%rsp)
	movq	-8(%rsp), %xmm8
	popq	 %r13
	xorpd	%xmm9, %xmm9
	movq	%r13, %xmm9
	subsd	%xmm9, %xmm8
	popq	 %r13
	xorpd	%xmm9, %xmm9
	movq	%r13, %xmm9
	divsd	%xmm9, %xmm8
	xorpd	%xmm0, %xmm0
	movq	%rax, %xmm0
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
	.size	tan, .-tan
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
	subq	$8, %rsp
	addq	$8, %r15
	subq	$8, %rsp
	addq	$8, %r15
	subq	$8, %rsp
	addq	$8, %r15
	subq	$8, %rsp
	addq	$8, %r15
	pushq	 %r15
	movq	$0x400c000000000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -48(%rbp)
	movq	$0x400c000000000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -64(%rbp)
	movq	$0x4016000000000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -56(%rbp)
	movq	$0x4012000000000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -72(%rbp)
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
	movq	-48(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %xmm0
	movq	-64(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %xmm1
	movq	-56(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %xmm2
	movq	-72(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	movq	%xmm8, %xmm3
	call	 tan
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
	movq	%xmm8, %xmm0
	movl	$1, %eax
	call	 printf
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
	.string	"tan = %f\n"

# end of generated code
