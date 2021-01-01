# start of generated code
	.text	
ind:
	.global	ind
	.type	ind, @function
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
	pushq	 %r15
	movq	$1, %rax
	movq	%rax, -48(%rbp)
	movq	%rdi, %rax
	movq	%rax, -56(%rbp)
.POS0:
	movq	$1, %rax
	pushq	 %rax
	movq	-56(%rbp), %rax
	popq	 %rbx
	xorq	%r13, %r13
	cmpq	%rax, %rbx
	setne	%r13b
	movq	%r13, %rax
	testq	%rax, %rax
	je	 .POS1
	xorq	%r15, %r15
	pushq	 %r15
	movq	%rsi, %rax
	pushq	 %rax
	movq	%rdi, %rax
	pushq	 %rax
	movq	-56(%rbp), %rax
	popq	 %rbx
	imulq	%rbx, %rax
	xorq	%rdx, %rdx
	popq	 %r13
	idivq	%r13
	movq	%rdx, %rax
	movq	%rax, -56(%rbp)
	movq	$1, %rax
	pushq	 %rax
	movq	-48(%rbp), %rax
	popq	 %rbx
	addq	%rbx, %rax
	movq	%rax, -48(%rbp)
	popq	 %r15
	addq	%r15, %rsp
	jmp	 .POS0
.POS1:
	movq	-48(%rbp), %rax
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
	.size	ind, .-ind
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
	pushq	 %r15
	movq	$1, %rax
	movq	%rax, -48(%rbp)
.POS2:
	movq	$23, %rax
	pushq	 %rax
	movq	-48(%rbp), %rax
	popq	 %rbx
	xorq	%r13, %r13
	cmpq	%rax, %rbx
	setg	%r13b
	movq	%r13, %rax
	testq	%rax, %rax
	je	 .POS3
	xorq	%r15, %r15
	pushq	 %r15
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
	movq	-48(%rbp), %rax
	movq	%rax, %rsi
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
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	movq	$23, %rax
	movq	%rax, %rsi
	call	 ind
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
	movq	%rax, %rdx
	movl	$0, %eax
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
	popq	 %r15
	addq	%r15, %rsp
.POS4:
	movq	$1, %rax
	pushq	 %rax
	movq	-48(%rbp), %rax
	popq	 %rbx
	addq	%rbx, %rax
	movq	%rax, -48(%rbp)
	jmp	 .POS2
.POS3:
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
	.string	"ind(%lld) = %lld \n"

# end of generated code
