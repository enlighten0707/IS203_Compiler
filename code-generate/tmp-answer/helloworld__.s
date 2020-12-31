# start of generated code
.LC0:
	.string	"Hello world!"
	.text	
.globl	main
.type	main, @function
main:
	pushq	 %rbp
	movq	%rsp, %rbp
	subq	$8, %rsp
	movq	$.LC0, %rax
	movq	%rax, %rdi
	subq	$8, %rsp
	movl	$0, %eax
	call	 printf
	leave	
	ret	

# end of generated code
