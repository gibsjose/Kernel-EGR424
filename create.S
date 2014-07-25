/*
 * Implement the thread creation task:
 *
 *   The C prototype for this function call is:
 *       createThread(threads[i].registers, threads[i].stack)
 *   thus:
 *       R0 <-- registers (the register array)
 *       R1 <-- stack (address of top-of-stack)
 */
    .syntax unified
    .text
    .align 2
    .thumb
    .thumb_func
    .type createThread,function
    .global createThread

createThread:

	@Store r4-r12 and PSP on array
	stm 	r0, {r1, r4-r12}

	@Initialize xPSR (first on stack) with 0x01000000 (Thumb bit set)
	movt 	r2, 0x0100
	movw  	r2, 0x0000
	stmdb 	r1!, {r2}	@Store xPSR at the highest memory location on the stack

	@Store threadStarter as addressToReturnTo (second on stack)
	ldr  	r2, .L0
	stmdb   r1!, {r2}	@Store addressToReturnTo (threadStarter) on second highest memory on stack

	@Initialize r0-r3, r12, and r14
	mov  	r0, #0
	mov  	r2, #0
	mov  	r3, #0
	mov  	r12, #0
	stmdb   r1!, {r0} 			@Simulate storing r14 as 0 by storing r0 instead
	stmdb 	r1!, {r2, r3, r12}	@Store r0-r3, r12 (IP), and r14 (LR) on stack
	stmdb   r1!, {r0, r2}		@Simulate storing r1 (0) on stack by storing a copy of r2

	bx		lr

.L0:
    .word   threadStarter
