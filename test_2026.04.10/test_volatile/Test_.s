	.file	"Test.c"
	.text
	.globl	quit
	.bss
	.align 4
	.type	quit, @object
	.size	quit, 4
quit:
	.zero	4
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	testGcc
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
.LC0:
	.string	"\346\255\243\345\270\270\351\200\200\345\207\272"
	.text
	.globl	testGcc
	.type	testGcc, @function
testGcc:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	testGccHandler(%rip), %rax
	movq	%rax, %rsi
	movl	$2, %edi
	call	signal@PLT
	nop
.L4:
	movl	quit(%rip), %eax
	testl	%eax, %eax
	je	.L4
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	testGcc, .-testGcc
	.section	.rodata
.LC1:
	.string	"%d\344\277\241\345\217\267\346\255\243\345\234\250\350\242\253\346\215\225\346\215\211\n"
.LC2:
	.string	"quit: %d"
.LC3:
	.string	"->%d\n"
	.text
	.globl	testGccHandler
	.type	testGccHandler, @function
testGccHandler:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	quit(%rip), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$1, quit(%rip)
	movl	quit(%rip), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	testGccHandler, .-testGccHandler
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
