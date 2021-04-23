	.file	"test.c"
# GNU C11 (Ubuntu 7.5.0-3ubuntu1~18.04) version 7.5.0 (x86_64-linux-gnu)
#	compiled by GNU C version 7.5.0, GMP version 6.1.2, MPFR version 4.0.1, MPC version 1.1.0, isl version isl-0.19-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed:  -imultiarch x86_64-linux-gnu test.c -mtune=generic
# -march=x86-64 -auxbase-strip test.s -O0 -fverbose-asm
# -fstack-protector-strong -Wformat -Wformat-security
# options enabled:  -fPIC -fPIE -faggressive-loop-optimizations
# -fasynchronous-unwind-tables -fauto-inc-dec -fchkp-check-incomplete-type
# -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
# -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
# -fchkp-use-static-bounds -fchkp-use-static-const-bounds
# -fchkp-use-wrappers -fcommon -fdelete-null-pointer-checks
# -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
# -ffp-int-builtin-inexact -ffunction-cse -fgcse-lm -fgnu-runtime
# -fgnu-unique -fident -finline-atomics -fira-hoist-pressure
# -fira-share-save-slots -fira-share-spill-slots -fivopts
# -fkeep-static-consts -fleading-underscore -flifetime-dse
# -flto-odr-type-merging -fmath-errno -fmerge-debug-strings -fpeephole
# -fplt -fprefetch-loop-arrays -freg-struct-return
# -fsched-critical-path-heuristic -fsched-dep-count-heuristic
# -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
# -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
# -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
# -fshow-column -fshrink-wrap-separate -fsigned-zeros
# -fsplit-ivs-in-unroller -fssa-backprop -fstack-protector-strong
# -fstdarg-opt -fstrict-volatile-bitfields -fsync-libcalls -ftrapping-math
# -ftree-cselim -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im
# -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
# -ftree-phiprop -ftree-reassoc -ftree-scev-cprop -funit-at-a-time
# -funwind-tables -fverbose-asm -fzero-initialized-in-bss
# -m128bit-long-double -m64 -m80387 -malign-stringops
# -mavx256-split-unaligned-load -mavx256-split-unaligned-store
# -mfancy-math-387 -mfp-ret-in-387 -mfxsr -mglibc -mieee-fp
# -mlong-double-80 -mmmx -mno-sse4 -mpush-args -mred-zone -msse -msse2
# -mstv -mtls-direct-seg-refs -mvzeroupper

	.text
	.local	num_threads
	.comm	num_threads,4,4
	.comm	a,8,8
	.comm	b,8,8
	.comm	c,8,8
	.globl	do_work
	.type	do_work, @function
do_work:
.LFB5:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)	# args, args
# test.c:14: 	int tid = *(int *) args;
	movq	-24(%rbp), %rax	# args, tmp105
	movl	(%rax), %eax	# MEM[(int *)args_20(D)], tmp106
	movl	%eax, -4(%rbp)	# tmp106, tid
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	movl	-4(%rbp), %eax	# tid, tmp107
	movl	%eax, -8(%rbp)	# tmp107, i
	jmp	.L2	#
.L3:
# test.c:17: 			c[i] = a[i] + b[i];
	movq	a(%rip), %rax	# a, a.0_1
	movl	-8(%rbp), %edx	# i, tmp108
	movslq	%edx, %rdx	# tmp108, _2
	salq	$2, %rdx	#, _3
	addq	%rdx, %rax	# _3, _4
	movss	(%rax), %xmm1	# *_4, _5
	movq	b(%rip), %rax	# b, b.1_6
	movl	-8(%rbp), %edx	# i, tmp109
	movslq	%edx, %rdx	# tmp109, _7
	salq	$2, %rdx	#, _8
	addq	%rdx, %rax	# _8, _9
	movss	(%rax), %xmm0	# *_9, _10
	movq	c(%rip), %rax	# c, c.2_11
	movl	-8(%rbp), %edx	# i, tmp110
	movslq	%edx, %rdx	# tmp110, _12
	salq	$2, %rdx	#, _13
	addq	%rdx, %rax	# _13, _14
	addss	%xmm1, %xmm0	# _5, _15
	movss	%xmm0, (%rax)	# _15, *_14
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.3_16
	addl	%eax, -8(%rbp)	# num_threads.3_16, i
.L2:
# test.c:16: 	for(i = tid ; i < 104; i+=num_threads){
	cmpl	$103, -8(%rbp)	#, i
	jle	.L3	#,
# test.c:19: 	return NULL;
	movl	$0, %eax	#, _23
# test.c:20: }
	popq	%rbp	#
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	do_work, .-do_work
	.section	.rodata
.LC0:
	.string	"#threads: %d\n"
.LC1:
	.string	"HEY"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$48, %rsp	#,
	movl	%edi, -36(%rbp)	# argc, argc
	movq	%rsi, -48(%rbp)	# argv, argv
# test.c:28: 	num_threads = 2;
	movl	$2, num_threads(%rip)	#, num_threads
# test.c:29: 	if(argc > 1){
	cmpl	$1, -36(%rbp)	#, argc
	jle	.L6	#,
# test.c:30: 		num_threads = atoi(argv[1]);
	movq	-48(%rbp), %rax	# argv, tmp138
	addq	$8, %rax	#, _1
	movq	(%rax), %rax	# *_1, _2
	movq	%rax, %rdi	# _2,
	call	atoi@PLT	#
	movl	%eax, num_threads(%rip)	# _3, num_threads
.L6:
# test.c:33: 	printf("#threads: %d\n" , num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.4_4
	movl	%eax, %esi	# num_threads.4_4,
	leaq	.LC0(%rip), %rdi	#,
	movl	$0, %eax	#,
	call	printf@PLT	#
# test.c:34: 	a = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
	movq	%rax, a(%rip)	# _5, a
# test.c:35: 	b = (float * ) malloc(500*sizeof(float));
	movl	$2000, %edi	#,
	call	malloc@PLT	#
	movq	%rax, b(%rip)	# _6, b
# test.c:36: 	c = (float * ) malloc(1000*sizeof(float));
	movl	$4000, %edi	#,
	call	malloc@PLT	#
	movq	%rax, c(%rip)	# _7, c
# test.c:39: 	srand(time(NULL));
	movl	$0, %edi	#,
	call	time@PLT	#
	movl	%eax, %edi	# _9,
	call	srand@PLT	#
# test.c:40: 	for(i = 0; i < 500; i++)
	movl	$0, -28(%rbp)	#, i
	jmp	.L7	#
.L8:
# test.c:42: 		a[i] = rand() % 100 -50; 
	call	rand@PLT	#
	movl	%eax, %ecx	#, _10
	movl	$1374389535, %edx	#, tmp143
	movl	%ecx, %eax	# _10, tmp166
	imull	%edx	# tmp143
	sarl	$5, %edx	#, tmp144
	movl	%ecx, %eax	# _10, tmp145
	sarl	$31, %eax	#, tmp145
	subl	%eax, %edx	# tmp145, tmp144
	movl	%edx, %eax	# tmp144, _11
	imull	$100, %eax, %eax	#, _11, tmp146
	subl	%eax, %ecx	# tmp146, _10
	movl	%ecx, %eax	# _10, _11
	leal	-50(%rax), %ecx	#, _12
	movq	a(%rip), %rax	# a, a.5_13
	movl	-28(%rbp), %edx	# i, tmp147
	movslq	%edx, %rdx	# tmp147, _14
	salq	$2, %rdx	#, _15
	addq	%rdx, %rax	# _15, _16
	cvtsi2ss	%ecx, %xmm0	# _12, _17
	movss	%xmm0, (%rax)	# _17, *_16
# test.c:43: 		b[i] = rand() % 100 -50; 
	call	rand@PLT	#
	movl	%eax, %ecx	#, _18
	movl	$1374389535, %edx	#, tmp149
	movl	%ecx, %eax	# _18, tmp167
	imull	%edx	# tmp149
	sarl	$5, %edx	#, tmp150
	movl	%ecx, %eax	# _18, tmp151
	sarl	$31, %eax	#, tmp151
	subl	%eax, %edx	# tmp151, tmp150
	movl	%edx, %eax	# tmp150, _19
	imull	$100, %eax, %eax	#, _19, tmp152
	subl	%eax, %ecx	# tmp152, _18
	movl	%ecx, %eax	# _18, _19
	leal	-50(%rax), %ecx	#, _20
	movq	b(%rip), %rax	# b, b.6_21
	movl	-28(%rbp), %edx	# i, tmp153
	movslq	%edx, %rdx	# tmp153, _22
	salq	$2, %rdx	#, _23
	addq	%rdx, %rax	# _23, _24
	cvtsi2ss	%ecx, %xmm0	# _20, _25
	movss	%xmm0, (%rax)	# _25, *_24
# test.c:40: 	for(i = 0; i < 500; i++)
	addl	$1, -28(%rbp)	#, i
.L7:
# test.c:40: 	for(i = 0; i < 500; i++)
	cmpl	$499, -28(%rbp)	#, i
	jle	.L8	#,
# test.c:45: 	int * thread_args = (int *) malloc(sizeof(int) * num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.7_26
	cltq
	salq	$2, %rax	#, _28
	movq	%rax, %rdi	# _28,
	call	malloc@PLT	#
	movq	%rax, -16(%rbp)	# tmp154, thread_args
# test.c:46: 	pthread_t * threads = (pthread_t * ) malloc(sizeof(pthread_t)  *  num_threads);
	movl	num_threads(%rip), %eax	# num_threads, num_threads.8_29
	cltq
	salq	$3, %rax	#, _31
	movq	%rax, %rdi	# _31,
	call	malloc@PLT	#
	movq	%rax, -8(%rbp)	# tmp155, threads
# test.c:47: 	for(int i = 0; i < num_threads ; i++){
	movl	$0, -24(%rbp)	#, i
	jmp	.L9	#
.L10:
# test.c:48: 		thread_args[i] = i;
	movl	-24(%rbp), %eax	# i, tmp156
	cltq
	leaq	0(,%rax,4), %rdx	#, _33
	movq	-16(%rbp), %rax	# thread_args, tmp157
	addq	%rax, %rdx	# tmp157, _34
	movl	-24(%rbp), %eax	# i, tmp158
	movl	%eax, (%rdx)	# tmp158, *_34
# test.c:49: 		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	movl	-24(%rbp), %eax	# i, tmp159
	cltq
	leaq	0(,%rax,4), %rdx	#, _36
	movq	-16(%rbp), %rax	# thread_args, tmp160
	addq	%rax, %rdx	# tmp160, _37
	movl	-24(%rbp), %eax	# i, tmp161
	cltq
	leaq	0(,%rax,8), %rcx	#, _39
	movq	-8(%rbp), %rax	# threads, tmp162
	addq	%rcx, %rax	# _39, _40
	movq	%rdx, %rcx	# _37,
	leaq	do_work(%rip), %rdx	#,
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _40,
	call	pthread_create@PLT	#
# test.c:47: 	for(int i = 0; i < num_threads ; i++){
	addl	$1, -24(%rbp)	#, i
.L9:
# test.c:47: 	for(int i = 0; i < num_threads ; i++){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.9_41
	cmpl	%eax, -24(%rbp)	# num_threads.9_41, i
	jl	.L10	#,
# test.c:51: 	printf("HEY");
	leaq	.LC1(%rip), %rdi	#,
	movl	$0, %eax	#,
	call	printf@PLT	#
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	movl	$0, -20(%rbp)	#, i
	jmp	.L11	#
.L12:
# test.c:55: 		pthread_join(threads[i],NULL);
	movl	-20(%rbp), %eax	# i, tmp163
	cltq
	leaq	0(,%rax,8), %rdx	#, _43
	movq	-8(%rbp), %rax	# threads, tmp164
	addq	%rdx, %rax	# _43, _44
	movq	(%rax), %rax	# *_44, _45
	movl	$0, %esi	#,
	movq	%rax, %rdi	# _45,
	call	pthread_join@PLT	#
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	addl	$1, -20(%rbp)	#, i
.L11:
# test.c:54: 	for(int i = 0; i < num_threads ; i++){
	movl	num_threads(%rip), %eax	# num_threads, num_threads.10_46
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
	movl	$0, %eax	#, _82
# test.c:64: }
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
