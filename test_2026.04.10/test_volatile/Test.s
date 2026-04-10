	.file	"Test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\344\277\241\345\217\267\346\255\243\345\234\250\350\242\253\346\215\225\346\215\211\n"
.LC1:
	.string	"quit: %d"
.LC2:
	.string	"->%d\n"
	.text
	.p2align 4
	.globl	testGccHandler
	.type	testGccHandler, @function
testGccHandler:
.LFB13:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	%edi, %esi
	leaq	.LC0(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	quit(%rip), %esi
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	call	printf@PLT
	movl	$1, %esi
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	movl	$1, quit(%rip)
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	printf@PLT
	.cfi_endproc
.LFE13:
	.size	testGccHandler, .-testGccHandler
	.section	.rodata.str1.1
.LC3:
	.string	"\346\255\243\345\270\270\351\200\200\345\207\272"
	.text
	.p2align 4
	.globl	testGcc
	.type	testGcc, @function
testGcc:
.LFB12:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	leaq	testGccHandler(%rip), %rsi
	movl	$2, %edi
	call	signal@PLT
	movl	quit(%rip), %eax
	testl	%eax, %eax
	jne	.L5
.L6:
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L5:
	leaq	.LC3(%rip), %rdi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	puts@PLT
	.cfi_endproc
.LFE12:
	.size	testGcc, .-testGcc
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	call	testGcc
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.globl	quit
	.bss
	.align 4
	.type	quit, @object
	.size	quit, 4
quit:
	.zero	4
	.ident	"GCC: (Debian 14.2.0-19) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
