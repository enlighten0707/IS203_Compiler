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
	subq	$8, %rsp
	addq	$8, %r15
	subq	$8, %rsp
	addq	$8, %r15
	subq	$8, %rsp
	addq	$8, %r15
	pushq	 %r15
	movq	$0x4008000000000000, %rax
	xorpd	%xmm8, %xmm8
	movq	%rax, %xmm8
	movsd	%xmm8, -48(%rbp)
	movq	$2551521, %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	xorpd	%xmm8, %xmm8
	cvtsi2sdq	%rax, %xmm8
	movq	%xmm8, %r12
	pushq	 %r12
	movq	-48(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	popq	 %r13
	xorpd	%xmm9, %xmm9
	movq	%r13, %xmm9
	xorq	%r13, %r13
	ucomisd	%xmm8, %xmm9
	setg	%r13b
	movq	%r13, %rax
	testq	%rax, %rax
	je	 .POS0
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
	jmp	 .POS1
.POS0:
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
	movq	$.LC1, %rax
	movq	%rax, %rdi
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
.POS1:
	movq	-56(%rbp), %rax
	xorpd	%xmm8, %xmm8
	cvtsi2sdq	%rax, %xmm8
	movq	%xmm8, %r12
	pushq	 %r12
	movq	-48(%rbp), %r12
	movq	%r12, -8(%rsp)
	xorpd	%xmm8, %xmm8
	movq	-8(%rsp), %xmm8
	popq	 %r13
	xorpd	%xmm9, %xmm9
	movq	%r13, %xmm9
	xorq	%r13, %r13
	ucomisd	%xmm8, %xmm9
	setg	%r13b
	movq	%r13, %rax
	movq	%rax, -64(%rbp)
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
	movq	$.LC2, %rax
	movq	%rax, %rdi
	movq	-64(%rbp), %rax
	movq	%rax, %rsi
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
	.string	"3.0<2551521\n"
.LC1:
	.string	"3.0>=2551521\n"
.LC2:
	.string	"y=%lld\n"

# end of generated code
