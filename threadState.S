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
 * extern void saveThreadState(unsigned *p_registers);
 */
saveThreadState:
    mrs r1, psp
    stm r0, {r1, r4-r12}
    bx lr

    .syntax unified
    .text
    .align 2
    .thumb
    .thumb_func
    .type restoreThreadState,function
    .global restoreThreadState

/*
 * extern void restoreThreadState(unsigned *p_registers);
 */
restoreThreadState:
    ldr r1, [r0]
    add r0, r0, #4
    msr psp, r1
    ldm r0, {r4-r12}
    bx lr
