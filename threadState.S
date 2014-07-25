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
    
    mov r2, 0xf000000d          ; Value for LR: Return to Thread Mode, exception return gets state from process stack
    stmea r0!, {r4-r11, r1, r2} ; Store R4-R11, PSP (R13), and LR

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
    bx lr

