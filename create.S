/*
 * Implement the thread creation task:
 *
 *   The C prototype for this function call is:
 *       createThread(threads[i].registers, &(threads[i].stack))
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

	@Store the actual process stack pointer (char*) in r2 from the char ** in r1
	ldr     r2, [r1]

	@Initialize xPSR (first on stack) with 0x01000000 (Thumb bit set)
	movw  	r3, 0x0000
	movt 	r3, 0x0100
	stmdb 	r2!, {r3}	@Store xPSR at the highest memory location on the stack

	@Store threadStarter as addressToReturnTo (second on stack)
	ldr  	r3, =threadStarter
	stmdb   r2!, {r3}	@Store addressToReturnTo (threadStarter) on second highest memory on stack

	@Initialize r0-r3, r12, and r14
	mov  	r3, #0
	mov  	r12, #0
	stmdb   r2!, {r3}	 		@Simulate storing r14 (LR) as 0
	stmdb 	r2!, {r3, r12}		@Store r3, r12 (IP) on stack
	stmdb   r2!, {r3} 			@Simulate storing r2 on the stack by using the value 0
	stmdb   r2!, {r3, r12}		@Simulate storing r0 (0) and r1 (0) on stack by storing zeros

	@Save the pointer to the stack pointer
	str r2, [r1]

	@Store r4-r12 and PSP on array
	stm 	r0, {r2, r4-r12}

	bx		lr