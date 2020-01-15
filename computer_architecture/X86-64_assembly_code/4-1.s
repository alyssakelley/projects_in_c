	.file	"4-1.c"
	.text
	.globl	loop
	.type	loop, @function
loop:
.LFB11:
	.cfi_startproc
	movq	%rsi, %rcx
	movl	$1, %edx
	movl	$0, %eax
.L2:
	testq	%rdx, %rdx
	je	.L4
	movq	%rdx, %r8
	andq	%rdi, %r8
	orq	%r8, %rax
	salq	%cl, %rdx
	jmp	.L2
.L4:
	ret
	.cfi_endproc
.LFE11:
	.size	loop, .-loop
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$5, %esi
	movl	$1, %edi
	call	loop
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$4, %esi
	movl	$2, %edi
	call	loop
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$3, %esi
	movl	$3, %edi
	call	loop
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$2, %esi
	movl	$4, %edi
	call	loop
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, %esi
	movl	$5, %edi
	call	loop
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.2.1 20180831"
	.section	.note.GNU-stack,"",@progbits
