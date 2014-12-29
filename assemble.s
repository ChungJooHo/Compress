	.text
	.globl	huffman_init
	.def	huffman_init;	.scl	2;	.type	32;	.endef
	.seh_proc	huffman_init
huffman_init:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$16, %rsp
	.seh_stackalloc	16
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	16(%rbp), %rax
	addq	%rax, %rdx
	movl	-4(%rbp), %eax
	movl	%eax, (%rdx)
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,8), %rdx
	movq	16(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, 4(%rax)
	addl	$1, -4(%rbp)
.L2:
	cmpl	$255, -4(%rbp)
	jle	.L3
	addq	$16, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	get_size
	.def	get_size;	.scl	2;	.type	32;	.endef
	.seh_proc	get_size
get_size:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movl	$0, %r8d
	movl	$0, %edx
	movq	16(%rbp), %rcx
	call	fseek
	movq	16(%rbp), %rcx
	call	feof
	testl	%eax, %eax
	je	.L5
	movl	$0, %eax
	jmp	.L6
.L5:
	movl	$0, -4(%rbp)
	jmp	.L7
.L8:
	addl	$1, -4(%rbp)
.L7:
	movq	16(%rbp), %rcx
	call	fgetc
	movq	16(%rbp), %rcx
	call	feof
	testl	%eax, %eax
	je	.L8
	movl	$0, %r8d
	movl	$0, %edx
	movq	16(%rbp), %rcx
	call	fseek
	movl	-4(%rbp), %eax
.L6:
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.globl	return_int
	.def	return_int;	.scl	2;	.type	32;	.endef
	.seh_proc	return_int
return_int:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, %eax
	movb	%al, 16(%rbp)
	cmpb	$97, 16(%rbp)
	jne	.L10
	movl	$0, %eax
	jmp	.L11
.L10:
	cmpb	$116, 16(%rbp)
	jne	.L12
	movl	$1, %eax
	jmp	.L11
.L12:
	cmpb	$103, 16(%rbp)
	jne	.L13
	movl	$2, %eax
	jmp	.L11
.L13:
	cmpb	$99, 16(%rbp)
	jne	.L14
	movl	$3, %eax
	jmp	.L11
.L14:
	movl	$-1, %eax
.L11:
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (tdm64-2) 4.8.1"
	.def	fseek;	.scl	2;	.type	32;	.endef
	.def	feof;	.scl	2;	.type	32;	.endef
	.def	fgetc;	.scl	2;	.type	32;	.endef
