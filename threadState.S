/*
 * Save/Restore Thread State
 *
 */
    .syntax unified
    .text
    .align 2
    .thumb
    .thumb_func
    .type saveThreadState,function
    .global saveThreadState


/*
 * void saveThreadState(unsigned *registers, char *stack)
 */
saveThreadState:

    stmea r0!, {r1, r4-r12} ; Store R4-R12 and PSP (R13)

    bx lr

    .syntax unified
    .text
    .align 2
    .thumb
    .thumb_func
    .type restoreThreadState,function
    .global restoreThreadState

/*
 * void restoreThreadState(unsigned *registers)
 */
restoreThreadState:
    ldria 	r1, [r0]! ;Load stack pointer from register array
    msr 	psp, r1   ;Set the process stack pointer to stac

    bx lr
