	.file	"test.c"
# GNU C17 (GCC) version 10.2.0 (x86_64-pc-linux-gnu)
#	compiled by GNU C version 10.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.21-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  test.c -mtune=generic -march=x86-64
# -auxbase-strip test.s -O0 -fverbose-asm
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -fallocation-dce -fasynchronous-unwind-tables -fauto-inc-dec
# -fdelete-null-pointer-checks -fdwarf2-cfi-asm -fearly-inlining
# -feliminate-unused-debug-symbols -feliminate-unused-debug-types
# -ffp-int-builtin-inexact -ffunction-cse -fgcse-lm -fgnu-unique -fident
# -finline-atomics -fipa-stack-alignment -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots -fivopts
# -fkeep-static-consts -fleading-underscore -flifetime-dse -fmath-errno
# -fmerge-debug-strings -fpeephole -fplt -fprefetch-loop-arrays
# -freg-struct-return -fsched-critical-path-heuristic
# -fsched-dep-count-heuristic -fsched-group-heuristic -fsched-interblock
# -fsched-last-insn-heuristic -fsched-rank-heuristic -fsched-spec
# -fsched-spec-insn-heuristic -fsched-stalled-insns-dep -fschedule-fusion
# -fsemantic-interposition -fshow-column -fshrink-wrap-separate
# -fsigned-zeros -fsplit-ivs-in-unroller -fssa-backprop
# -fstack-protector-strong -fstdarg-opt -fstrict-volatile-bitfields
# -fsync-libcalls -ftrapping-math -ftree-cselim -ftree-forwprop
# -ftree-loop-if-convert -ftree-loop-im -ftree-loop-ivcanon
# -ftree-loop-optimize -ftree-parallelize-loops= -ftree-phiprop
# -ftree-reassoc -ftree-scev-cprop -funit-at-a-time -funwind-tables
# -fverbose-asm -fzero-initialized-in-bss -m128bit-long-double -m64 -m80387
# -malign-stringops -mavx256-split-unaligned-load
# -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387 -mfxsr
# -mglibc -mieee-fp -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone
# -msse -msse2 -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
	.globl	a
	.bss
	.align 8
	.type	a, @object
	.size	a, 8
a:
	.zero	8
	.globl	b
	.align 8
	.type	b, @object
	.size	b, 8
b:
	.zero	8
	.globl	c
	.align 8
	.type	c, @object
	.size	c, 8
c:
	.zero	8
	.section	.rodata
.LC0:
	.string	"hello tid: %d \n"
	.text
	.globl	do_work
	.type	do_work, @function
do_work:
.LFB6:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$32, %rsp	#,
	movq	%rdi, -24(%rbp)	# args, args
# test.c:14: 	int tid = *(int *) args;
	movq	-24(%rbp), %rax	# args, tmp93
	movl	(%rax), %eax	# MEM[(int *)args_13(D)], tmp94
	movl	%eax, -4(%rbp)	# tmp94, tid
# test.c:16: 	for(i = tid ; i < 100; i+=num_threads){
	movl	-4(%rbp), %eax	# tid, tmp95
	movl	%eax, -8(%rbp)	# tmp95, i
# test.c:16: 	for(i = tid ; i < 100; i+=num_threads){
	jmp	.L2	#
.L3:
# test.c:17: 			c[i] = a[i] ;
	movq	a(%rip), %rdx	# a, a.0_1
	movl	-8(%rbp), %eax	# i, tmp96
	cltq
	salq	$2, %rax	#, _3
	addq	%rax, %rdx	# _3, _4
# test.c:17: 			c[i] = a[i] ;
	movq	c(%rip), %rcx	# c, c.1_5
	movl	-8(%rbp), %eax	# i, tmp97
	cltq
	salq	$2, %rax	#, _7
	addq	%rcx, %rax	# c.1_5, _8
# test.c:17: 			c[i] = a[i] ;
	movss	(%rdx), %xmm0	# *_4, _9
# test.c:17: 			c[i] = a[i] ;
	movss	%xmm0, (%rax)	# _9, *_8
# test.c:16: 	for(i = tid ; i < 100; i+=num_threads){
	addl	$2, -8(%rbp)	#, i
.L2:
# test.c:16: 	for(i = tid ; i < 100; i+=num_threads){
	cmpl	$99, -8(%rbp)	#, i
	jle	.L3	#,
# test.c:19: 	printf("hello tid: %d \n",tid);
	movl	-4(%rbp), %eax	# tid, tmp98
	movl	%eax, %esi	# tmp98,
	leaq	.LC0(%rip), %rdi	#,
	movl	$0, %eax	#,
	call	printf@PLT	#
# test.c:20: 	return NULL;
	movl	$0, %eax	#, _17
# test.c:21: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	do_work, .-do_work
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$80, %rsp	#,
	movl	%edi, -68(%rbp)	# argc, argc
	movq	%rsi, -80(%rbp)	# argv, argv
# test.c:28: {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp143
	movq	%rax, -8(%rbp)	# tmp143, D.3391
	xorl	%eax, %eax	# tmp143
# test.c:29: 	a = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
# test.c:29: 	a = (float * ) malloc(500*sizeof(float));
	movq	%rax, a(%rip)	# _1, a
# test.c:30: 	b = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
# test.c:30: 	b = (float * ) malloc(500*sizeof(float));
	movq	%rax, b(%rip)	# _2, b
# test.c:31: 	c = (float * ) malloc(1000*sizeof(float));
	movl	$4000, %edi	#,
	call	malloc@PLT	#
# test.c:31: 	c = (float * ) malloc(1000*sizeof(float));
	movq	%rax, c(%rip)	# _3, c
# test.c:34: 	srand(time(NULL));
	movl	$0, %edi	#,
	call	time@PLT	#
# test.c:34: 	srand(time(NULL));
	movl	%eax, %edi	# _5,
	call	srand@PLT	#
# test.c:35: 	for(i = 0; i < 500; i++)
	movl	$0, -52(%rbp)	#, i
# test.c:35: 	for(i = 0; i < 500; i++)
	jmp	.L6	#
.L7:
# test.c:37: 		a[i] = rand() % 100 -50; 
	call	rand@PLT	#
# test.c:37: 		a[i] = rand() % 100 -50; 
	movslq	%eax, %rdx	# _6, tmp114
	imulq	$1374389535, %rdx, %rdx	#, tmp114, tmp115
	shrq	$32, %rdx	#, tmp116
	sarl	$5, %edx	#, tmp117
	movl	%eax, %ecx	# _6, tmp118
	sarl	$31, %ecx	#, tmp118
	subl	%ecx, %edx	# tmp118, _7
	imull	$100, %edx, %ecx	#, _7, tmp119
	subl	%ecx, %eax	# tmp119, _6
	movl	%eax, %edx	# _6, _7
# test.c:37: 		a[i] = rand() % 100 -50; 
	leal	-50(%rdx), %ecx	#, _8
# test.c:37: 		a[i] = rand() % 100 -50; 
	movq	a(%rip), %rdx	# a, a.2_9
	movl	-52(%rbp), %eax	# i, tmp120
	cltq
	salq	$2, %rax	#, _11
	addq	%rdx, %rax	# a.2_9, _12
# test.c:37: 		a[i] = rand() % 100 -50; 
	pxor	%xmm0, %xmm0	# _13
	cvtsi2ssl	%ecx, %xmm0	# _8, _13
	movss	%xmm0, (%rax)	# _13, *_12
# test.c:38: 		b[i] = rand() % 100 -50; 
	call	rand@PLT	#
# test.c:38: 		b[i] = rand() % 100 -50; 
	movslq	%eax, %rdx	# _14, tmp121
	imulq	$1374389535, %rdx, %rdx	#, tmp121, tmp122
	shrq	$32, %rdx	#, tmp123
	sarl	$5, %edx	#, tmp124
	movl	%eax, %ecx	# _14, tmp125
	sarl	$31, %ecx	#, tmp125
	subl	%ecx, %edx	# tmp125, _15
	imull	$100, %edx, %ecx	#, _15, tmp126
	subl	%ecx, %eax	# tmp126, _14
	movl	%eax, %edx	# _14, _15
# test.c:38: 		b[i] = rand() % 100 -50; 
	leal	-50(%rdx), %ecx	#, _16
# test.c:38: 		b[i] = rand() % 100 -50; 
	movq	b(%rip), %rdx	# b, b.3_17
	movl	-52(%rbp), %eax	# i, tmp127
	cltq
	salq	$2, %rax	#, _19
	addq	%rdx, %rax	# b.3_17, _20
# test.c:38: 		b[i] = rand() % 100 -50; 
	pxor	%xmm0, %xmm0	# _21
	cvtsi2ssl	%ecx, %xmm0	# _16, _21
	movss	%xmm0, (%rax)	# _21, *_20
# test.c:35: 	for(i = 0; i < 500; i++)
	addl	$1, -52(%rbp)	#, i
.L6:
# test.c:35: 	for(i = 0; i < 500; i++)
	cmpl	$499, -52(%rbp)	#, i
	jle	.L7	#,
# test.c:42: 	for(int i = 1; i < num_threads ; i++){
	movl	$1, -48(%rbp)	#, i
# test.c:42: 	for(int i = 1; i < num_threads ; i++){
	jmp	.L8	#
.L9:
# test.c:43: 		thread_args[i] = i;
	movl	-48(%rbp), %eax	# i, tmp129
	cltq
	movl	-48(%rbp), %edx	# i, tmp130
	movl	%edx, -40(%rbp,%rax,4)	# tmp130, thread_args[i_29]
# test.c:44: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	leaq	-40(%rbp), %rdx	#, tmp131
	movl	-48(%rbp), %eax	# i, tmp133
	cltq
	salq	$2, %rax	#, tmp134
	addq	%rax, %rdx	# tmp134, _22
# test.c:44: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	leaq	-32(%rbp), %rcx	#, tmp135
	movl	-48(%rbp), %eax	# i, tmp137
	cltq
	salq	$3, %rax	#, tmp138
	addq	%rcx, %rax	# tmp135, _23
	movq	%rdx, %rcx	# _22,
	leaq	do_work(%rip), %rdx	#,
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _23,
	call	pthread_create@PLT	#
# test.c:42: 	for(int i = 1; i < num_threads ; i++){
	addl	$1, -48(%rbp)	#, i
.L8:
# test.c:42: 	for(int i = 1; i < num_threads ; i++){
	cmpl	$1, -48(%rbp)	#, i
	jle	.L9	#,
# test.c:46: 	thread_args[0] = 0;
	movl	$0, -40(%rbp)	#, thread_args[0]
# test.c:47: 	do_work(&thread_args[0]);
	leaq	-40(%rbp), %rax	#, tmp139
	movq	%rax, %rdi	# tmp139,
	call	do_work	#
# test.c:48: 	for(int i = 1; i < num_threads ; i++){
	movl	$1, -44(%rbp)	#, i
# test.c:48: 	for(int i = 1; i < num_threads ; i++){
	jmp	.L10	#
.L11:
# test.c:49: 		pthread_join(threads[i],NULL);
	movl	-44(%rbp), %eax	# i, tmp141
	cltq
	movq	-32(%rbp,%rax,8), %rax	# threads[i_30], _24
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _24,
	call	pthread_join@PLT	#
# test.c:48: 	for(int i = 1; i < num_threads ; i++){
	addl	$1, -44(%rbp)	#, i
.L10:
# test.c:48: 	for(int i = 1; i < num_threads ; i++){
	cmpl	$1, -44(%rbp)	#, i
	jle	.L11	#,
# test.c:52: 	free(a);
	movq	a(%rip), %rax	# a, a.4_25
	movq	%rax, %rdi	# a.4_25,
	call	free@PLT	#
# test.c:53: 	free(b);
	movq	b(%rip), %rax	# b, b.5_26
	movq	%rax, %rdi	# b.5_26,
	call	free@PLT	#
# test.c:54: 	free(c);
	movq	c(%rip), %rax	# c, c.6_27
	movq	%rax, %rdi	# c.6_27,
	call	free@PLT	#
# test.c:57: 	return 0;
	movl	$0, %eax	#, _51
# test.c:58: }
	movq	-8(%rbp), %rsi	# D.3391, tmp144
	subq	%fs:40, %rsi	# MEM[(<address-space-1> long unsigned int *)40B], tmp144
	je	.L13	#,
	call	__stack_chk_fail@PLT	#
.L13:
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
