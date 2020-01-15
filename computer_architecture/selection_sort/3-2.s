	.file	"3-2.c"
	.text
	.globl	decode
	.type	decode, @function
decode:
.LFB11:
	.cfi_startproc
	subq	%rdx, %rsi
	movq	%rdi, %rax
	imulq	%rsi, %rax
	salq	$63, %rsi
	sarq	$63, %rsi
	xorl	%eax, %esi
	movslq	%esi, %rax
	ret
	.cfi_endproc
.LFE11:
	.size	decode, .-decode
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
	movl	$4, %edx
	movl	$2, %esi
	movl	$1, %edi
	call	decode
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$7, %edx
	movl	$5, %esi
	movl	$3, %edi
	call	decode
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$30, %edx
	movl	$20, %esi
	movl	$10, %edi
	call	decode
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
