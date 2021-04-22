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
	.local	num_threads
	.comm	num_threads,4,4
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
	movq	-24(%rbp), %rax	# args, tmp100
	movl	(%rax), %eax	# MEM[(int *)args_20(D)], tmp101
	movl	%eax, -4(%rbp)	# tmp101, tid
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	movl	-4(%rbp), %eax	# tid, tmp102
	movl	%eax, -8(%rbp)	# tmp102, i
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	jmp	.L2	#
.L3:
# test.c:17: 			c[i] = a[i] + b[i];
	movq	a(%rip), %rdx	# a, a.0_1
	movl	-8(%rbp), %eax	# i, tmp103
	cltq
	salq	$2, %rax	#, _3
	addq	%rdx, %rax	# a.0_1, _4
	movss	(%rax), %xmm1	# *_4, _5
# test.c:17: 			c[i] = a[i] + b[i];
	movq	b(%rip), %rdx	# b, b.1_6
	movl	-8(%rbp), %eax	# i, tmp104
	cltq
	salq	$2, %rax	#, _8
	addq	%rdx, %rax	# b.1_6, _9
	movss	(%rax), %xmm0	# *_9, _10
# test.c:17: 			c[i] = a[i] + b[i];
	movq	c(%rip), %rdx	# c, c.2_11
	movl	-8(%rbp), %eax	# i, tmp105
	cltq
	salq	$2, %rax	#, _13
	addq	%rdx, %rax	# c.2_11, _14
# test.c:17: 			c[i] = a[i] + b[i];
	addss	%xmm1, %xmm0	# _5, _15
# test.c:17: 			c[i] = a[i] + b[i];
	movss	%xmm0, (%rax)	# _15, *_14
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.3_16
	addl	%eax, -8(%rbp)	# num_threads.3_16, i
.L2:
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	cmpl	$103, -8(%rbp)	#, i
	jle	.L3	#,
# test.c:19: 	printf("hello tid: %d \n",tid);
	movl	-4(%rbp), %eax	# tid, tmp106
	movl	%eax, %esi	# tmp106,
	leaq	.LC0(%rip), %rdi	#,
	movl	$0, %eax	#,
	call	printf@PLT	#
# test.c:20: 	return NULL;
	movl	$0, %eax	#, _24
# test.c:21: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE6:
	.size	do_work, .-do_work
	.section	.rodata
.LC1:
	.string	"#threads: %d\n"
	.text
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
	subq	$48, %rsp	#,
	movl	%edi, -36(%rbp)	# argc, argc
	movq	%rsi, -48(%rbp)	# argv, argv
# test.c:29: 	num_threads = 2;
	movl	$2, num_threads(%rip)	#, num_threads
# test.c:30: 	if(argc > 1){
	cmpl	$1, -36(%rbp)	#, argc
	jle	.L6	#,
# test.c:31: 		num_threads = atoi(argv[1]);
	movq	-48(%rbp), %rax	# argv, tmp133
	addq	$8, %rax	#, _1
# test.c:31: 		num_threads = atoi(argv[1]);
	movq	(%rax), %rax	# *_1, _2
	movq	%rax, %rdi	# _2,
	call	atoi@PLT	#
# test.c:31: 		num_threads = atoi(argv[1]);
	movl	%eax, num_threads(%rip)	# _3, num_threads
.L6:
# test.c:34: 	printf("#threads: %d\n" , num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.4_4
	movl	%eax, %esi	# num_threads.4_4,
	leaq	.LC1(%rip), %rdi	#,
	movl	$0, %eax	#,
	call	printf@PLT	#
# test.c:35: 	a = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
# test.c:35: 	a = (float * ) malloc(500*sizeof(float));
	movq	%rax, a(%rip)	# _5, a
# test.c:36: 	b = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
# test.c:36: 	b = (float * ) malloc(500*sizeof(float));
	movq	%rax, b(%rip)	# _6, b
# test.c:37: 	c = (float * ) malloc(1000*sizeof(float));
	movl	$4000, %edi	#,
	call	malloc@PLT	#
# test.c:37: 	c = (float * ) malloc(1000*sizeof(float));
	movq	%rax, c(%rip)	# _7, c
# test.c:40: 	srand(time(NULL));
	movl	$0, %edi	#,
	call	time@PLT	#
# test.c:40: 	srand(time(NULL));
	movl	%eax, %edi	# _9,
	call	srand@PLT	#
# test.c:41: 	for(i = 0; i < 500; i++)
	movl	$0, -28(%rbp)	#, i
# test.c:41: 	for(i = 0; i < 500; i++)
	jmp	.L7	#
.L8:
# test.c:43: 		a[i] = rand() % 100 -50; 
	call	rand@PLT	#
# test.c:43: 		a[i] = rand() % 100 -50; 
	movslq	%eax, %rdx	# _10, tmp137
	imulq	$1374389535, %rdx, %rdx	#, tmp137, tmp138
	shrq	$32, %rdx	#, tmp139
	sarl	$5, %edx	#, tmp140
	movl	%eax, %ecx	# _10, tmp141
	sarl	$31, %ecx	#, tmp141
	subl	%ecx, %edx	# tmp141, _11
	imull	$100, %edx, %ecx	#, _11, tmp142
	subl	%ecx, %eax	# tmp142, _10
	movl	%eax, %edx	# _10, _11
# test.c:43: 		a[i] = rand() % 100 -50; 
	leal	-50(%rdx), %ecx	#, _12
# test.c:43: 		a[i] = rand() % 100 -50; 
	movq	a(%rip), %rdx	# a, a.5_13
	movl	-28(%rbp), %eax	# i, tmp143
	cltq
	salq	$2, %rax	#, _15
	addq	%rdx, %rax	# a.5_13, _16
# test.c:43: 		a[i] = rand() % 100 -50; 
	pxor	%xmm0, %xmm0	# _17
	cvtsi2ssl	%ecx, %xmm0	# _12, _17
	movss	%xmm0, (%rax)	# _17, *_16
# test.c:44: 		b[i] = rand() % 100 -50; 
	call	rand@PLT	#
# test.c:44: 		b[i] = rand() % 100 -50; 
	movslq	%eax, %rdx	# _18, tmp144
	imulq	$1374389535, %rdx, %rdx	#, tmp144, tmp145
	shrq	$32, %rdx	#, tmp146
	sarl	$5, %edx	#, tmp147
	movl	%eax, %ecx	# _18, tmp148
	sarl	$31, %ecx	#, tmp148
	subl	%ecx, %edx	# tmp148, _19
	imull	$100, %edx, %ecx	#, _19, tmp149
	subl	%ecx, %eax	# tmp149, _18
	movl	%eax, %edx	# _18, _19
# test.c:44: 		b[i] = rand() % 100 -50; 
	leal	-50(%rdx), %ecx	#, _20
# test.c:44: 		b[i] = rand() % 100 -50; 
	movq	b(%rip), %rdx	# b, b.6_21
	movl	-28(%rbp), %eax	# i, tmp150
	cltq
	salq	$2, %rax	#, _23
	addq	%rdx, %rax	# b.6_21, _24
# test.c:44: 		b[i] = rand() % 100 -50; 
	pxor	%xmm0, %xmm0	# _25
	cvtsi2ssl	%ecx, %xmm0	# _20, _25
	movss	%xmm0, (%rax)	# _25, *_24
# test.c:41: 	for(i = 0; i < 500; i++)
	addl	$1, -28(%rbp)	#, i
.L7:
# test.c:41: 	for(i = 0; i < 500; i++)
	cmpl	$499, -28(%rbp)	#, i
	jle	.L8	#,
# test.c:46: 	int * thread_args = (int *) malloc(sizeof(int) * num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.7_26
	cltq
	salq	$2, %rax	#, _28
	movq	%rax, %rdi	# _28,
	call	malloc@PLT	#
	movq	%rax, -16(%rbp)	# tmp151, thread_args
# test.c:47: 	pthread_t * threads = (pthread_t * ) malloc(sizeof(pthread_t)  *  num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.8_29
	cltq
	salq	$3, %rax	#, _31
	movq	%rax, %rdi	# _31,
	call	malloc@PLT	#
	movq	%rax, -8(%rbp)	# tmp152, threads
# test.c:48: 	for(int i = 0; i < num_threads ; i++){
	movl	$0, -24(%rbp)	#, i
# test.c:48: 	for(int i = 0; i < num_threads ; i++){
	jmp	.L9	#
.L10:
# test.c:49: 		thread_args[i] = i;
	movl	-24(%rbp), %eax	# i, tmp153
	cltq
	leaq	0(,%rax,4), %rdx	#, _33
	movq	-16(%rbp), %rax	# thread_args, tmp154
	addq	%rax, %rdx	# tmp154, _34
# test.c:49: 		thread_args[i] = i;
	movl	-24(%rbp), %eax	# i, tmp155
	movl	%eax, (%rdx)	# tmp155, *_34
# test.c:50: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	movl	-24(%rbp), %eax	# i, tmp156
	cltq
	leaq	0(,%rax,4), %rdx	#, _36
# test.c:50: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	movq	-16(%rbp), %rax	# thread_args, tmp157
	addq	%rax, %rdx	# tmp157, _37
# test.c:50: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	movl	-24(%rbp), %eax	# i, tmp158
	cltq
	leaq	0(,%rax,8), %rcx	#, _39
# test.c:50: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	movq	-8(%rbp), %rax	# threads, tmp159
	addq	%rcx, %rax	# _39, _40
	movq	%rdx, %rcx	# _37,
	leaq	do_work(%rip), %rdx	#,
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _40,
	call	pthread_create@PLT	#
# test.c:48: 	for(int i = 0; i < num_threads ; i++){
	addl	$1, -24(%rbp)	#, i
.L9:
# test.c:48: 	for(int i = 0; i < num_threads ; i++){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.9_41
# test.c:48: 	for(int i = 0; i < num_threads ; i++){
	cmpl	%eax, -24(%rbp)	# num_threads.9_41, i
	jl	.L10	#,
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	movl	$0, -20(%rbp)	#, i
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	jmp	.L11	#
.L12:
# test.c:55: 		pthread_join(threads[i],NULL);
	movl	-20(%rbp), %eax	# i, tmp160
	cltq
	leaq	0(,%rax,8), %rdx	#, _43
	movq	-8(%rbp), %rax	# threads, tmp161
	addq	%rdx, %rax	# _43, _44
# test.c:55: 		pthread_join(threads[i],NULL);
	movq	(%rax), %rax	# *_44, _45
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _45,
	call	pthread_join@PLT	#
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	addl	$1, -20(%rbp)	#, i
.L11:
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.10_46
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	cmpl	%eax, -20(%rbp)	# num_threads.10_46, i
	jl	.L12	#,
# test.c:58: 	free(a);
	movq	a(%rip), %rax	# a, a.11_47
	movq	%rax, %rdi	# a.11_47,
	call	free@PLT	#
# test.c:59: 	free(b);
	movq	b(%rip), %rax	# b, b.12_48
	movq	%rax, %rdi	# b.12_48,
	call	free@PLT	#
# test.c:60: 	free(c);
	movq	c(%rip), %rax	# c, c.13_49
	movq	%rax, %rdi	# c.13_49,
	call	free@PLT	#
# test.c:63: 	return 0;
	movl	$0, %eax	#, _81
# test.c:64: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (GNU) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
